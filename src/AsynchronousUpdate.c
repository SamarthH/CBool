#include "AsynchronousUpdate.h"
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include <gsl/gsl_rng.h>
#include <omp.h>

asynclist* updateAsync(int n, int** topology, int* fixed_nodes)
{
	asynclist* stableList = (asynclist*)malloc(sizeof(asynclist));
	init_asynclist(stableList);

	gsl_rng* state_maker = gsl_rng_alloc(gsl_rng_ranlxs2);
	gsl_rng_set(state_maker,0); // Using seed 0
	
	//#pragma omp target teams distribute parallel for
	#pragma omp parallel for
	for (int sample = 0; sample < N_SAMPLES; ++sample)
	{
		int state[n];
		//Get Initial State
		#pragma omp critical(state_randomize)
		{
			for (int i = 0; i < n; ++i)
			{
				if(fixed_nodes[i] == -1)
				{
					state[i] = gsl_rng_uniform_int(state_maker,2);
				}
				else{
					state[i] = fixed_nodes[i];
				}
			}
		}

		//Perform Asynchronous Updates
		gsl_rng* asyncer = gsl_rng_alloc(gsl_rng_ranlxs2);
		gsl_rng_set(asyncer,sample);

		for (int i = 0; i < ITER_ASYNCHRONOUS; ++i)
		{
			int node_to_update = gsl_rng_uniform_int(asyncer,n);
			int val_temp = 0;
			for (int j = 0; j < n; ++j)
			{
				val_temp+= state[node_to_update]*topology[node_to_update][j];
			}
			state[node_to_update] = (val_temp>0) + (val_temp==0)*state[node_to_update];
		}
		//Free the rng
		gsl_rng_free(asyncer);

		//Add the satte to list
		#pragma omp critical(addToList)
		{
			add_asynclist(stableList,state,n);
		}
	}

	gsl_rng_free(state_maker);
	return stableList;
}
