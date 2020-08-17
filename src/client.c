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
		asynclist* stable;
		stable = updateAsync(n_nodes,topology,fixed_nodes);
		print_asynclist(stable,n_nodes);
		clear_asynclist(stable);
	}

	for (int i = 0; i < n_nodes; ++i)
	{
		free(topology[i]);
	}
	free(topology);

	free(fixed_nodes);

	return 0;
}