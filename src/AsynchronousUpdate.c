#include "AsynchronousUpdate.h"
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include <gsl/gsl_rng.h>
#include <omp.h>

int getBool(gsl_rng* r)
{
	return gsl_rng_uniform_int(r,2);
}

void singleStateUpdate(int n, int* state, int** topology, int seed)
{
	//Perform Asynchronous Updates
	gsl_rng* asyncer = gsl_rng_alloc(gsl_rng_ranlxs2);
	gsl_rng_set(asyncer,seed);

	for (int i = 0; i < ITER_ASYNCHRONOUS; ++i)
	{
		int node_to_update = gsl_rng_uniform_int(asyncer,n);
		int val_temp = 0;
		#pragma omp parallel for reduction(+:val_temp)
		for (int j = 0; j < n; ++j)
		{
			val_temp+= state[j]*topology[node_to_update][j];
		}
		state[node_to_update] = (val_temp>0) + (val_temp==0)*state[node_to_update];
	}
	//Free the rng
	gsl_rng_free(asyncer);
}

void getFinalState(int n, int state[n], gsl_rng* rangen, int** topology, int* fixed_nodes, int i)
{
	#pragma omp parallel for
	for (int j = 0; j < n; ++j)
	{
		if(fixed_nodes[j] == -1)
		{
			#pragma omp critical(rand)
			{
				state[j] = getBool(rangen);
			}
		}
		else{
			state[j] = fixed_nodes[j];
		}
	}

	singleStateUpdate(n,state,topology,i+1);
}

asynclist* updateAsyncList(int n, int** topology, int* fixed_nodes, int height, int seed_init)
{
	if(height)
	{
		if(height%2)
		{
			gsl_rng* rangen = gsl_rng_alloc(gsl_rng_ranlxs2);
			gsl_rng_set(rangen,seed_init);
			asynclist* to_merge[2];
			int seed[2] = {gsl_rng_get(rangen),gsl_rng_get(rangen)};
			int i;
			#pragma omp parallel for shared(n,topology,fixed_nodes,seed,height) private(i)
			for (i = 0; i < 2; ++i)
			{
				to_merge[i] = updateAsyncList(n,topology,fixed_nodes,height-1,seed[i]);
			}
			return merge_asynclist(to_merge[0],to_merge[1],n); 
		}
		else
		{
			gsl_rng* rangen = gsl_rng_alloc(gsl_rng_taus2);
			gsl_rng_set(rangen,seed_init);
			asynclist* to_merge[2];
			int seed[2] = {gsl_rng_get(rangen),gsl_rng_get(rangen)};
			int i;
			#pragma omp parallel for shared(n,topology,fixed_nodes,seed,height) private(i)
			for (i = 0; i < 2; ++i)
			{
				to_merge[i] = updateAsyncList(n,topology,fixed_nodes,height-1,seed[i]);
			}
			return merge_asynclist(to_merge[0],to_merge[1],n); 
		}
	}
	else{
		asynclist* list = (asynclist*)malloc(sizeof(asynclist));
		init_asynclist(list);
		int state[n];

		gsl_rng* rangen = gsl_rng_alloc(gsl_rng_ranlxs2);
		gsl_rng_set(rangen,seed_init);

		for (int i = 0; i < n; ++i)
		{
			if(fixed_nodes[i] == -1)
			{
				state[i] = gsl_rng_uniform_int(rangen,2);
			}
			else
			{
				state[i] = fixed_nodes[i];
			}
		}

		singleStateUpdate(n,state,topology,gsl_rng_get(rangen));
		list->n_elements = 1;
		asynclistnode* node = (asynclistnode*)malloc(sizeof(asynclistnode));
		if(!node)
		{
			exit(10);
		}
		node->n_occurances = 1;
		node->state = (int*)malloc(n*sizeof(int));
		if(!(node->state))
		{
			exit(11);
		}
		for (int i = 0; i < n; ++i)
		{
			node->state[i] = state[i];
		}
		node->next = NULL;
		node->prev = NULL;
		list->next = node;
		return list;
	}
}

void updateAsyncTree(int n, int** topology, int* fixed_nodes, base* stable)
{
	int iter = (1<<N_SAMPLES);
	
	gsl_rng* rangen = gsl_rng_alloc(gsl_rng_ranlxs2);
	gsl_rng_set(rangen,0);
	
	#pragma omp parallel for
	for (int i = 0; i < iter; ++i)
	{
		/*
		gsl_rng* rangen = gsl_rng_alloc(gsl_rng_ranlxs2);
		gsl_rng_set(rangen,i);
		*/
		//int* state=(int*)malloc(n*sizeof(int));
		int state[n];
		getFinalState(n,state,rangen,topology,fixed_nodes,i);

		#pragma omp critical(addToTree)
		{
			add_node(stable,n,state);
		}
		//free(state);
	}
	gsl_rng_free(rangen);
}