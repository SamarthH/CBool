#include <stdio.h>
#include <stdlib.h>

#include "requirements.h"
#include "constants.h"

int main()
{
	int** topology;
	char interaction_type; //Synchronous = 's', async = 'a'
	int n_nodes;
	int* fixed_nodes;

	getInput(&n_nodes, &topology, &interaction_type, &fixed_nodes);

	if(interaction_type=='a')
	{
		if(0)
		{
			asynclist* stable;
			stable = updateAsyncList(n_nodes,topology,fixed_nodes,N_SAMPLES,0);
			print_asynclist(stable,n_nodes);
			clear_asynclist(stable);
		}
		else{
			base stable;
			init_tree(&stable);
			printf("k\n");
			updateAsyncTree(n_nodes,topology,fixed_nodes,&stable);
			print_tree(&stable,n_nodes);
			delete_tree(&stable);
		}
	}

	for (int i = 0; i < n_nodes; ++i)
	{
		free(topology[i]);
	}
	free(topology);

	free(fixed_nodes);

	return 0;
}