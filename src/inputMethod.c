#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputMethod.h"

void getInput(int* n_nodes, int*** topology, char* interaction_type, int** fixed_nodes)
{
	char strength;
	char query[30];

	while(1)
	{
		scanf("%s",query);
		if(strcmp(query,"INTERACTION_STRENGTH")==0)
		{
			char input[20];
			scanf("%s",input);
			if(strcmp(input,"ISING")==0){
				strength = 'n';
			}
			else if(strcmp(input,"DOM_ACT")==0){ // Activatory Dominant
				strength = 'a';
			}
			else if(strcmp(input,"DOM_INHIB")==0){ // Inhibitory Dominant
				strength = 'a';
			}
			else{
				exit(3);
			}
		}
		else if(strcmp(query,"INTERACTION_TYPE")==0)
		{
			char input[20];
			scanf("%s",input);
			if(strcmp(input,"SYNC")==0){
				*interaction_type = 's';
			}
			else if(strcmp(input,"ASYNC")==0){
				*interaction_type = 'a';
			}
			else{
				exit(4);
			}	
		}
		else if(strcmp(query,"NODES")==0)
		{
			scanf("%d",n_nodes);
			*topology = (int**) malloc(*n_nodes*sizeof(int*));
			if(!(*topology)){
				exit(5);
			}
			for (int i = 0; i < *n_nodes; ++i)
			{
				(*topology)[i] = (int*)malloc((*n_nodes)*sizeof(int));
				if(!((*topology)[i])){
					exit(6);
				}
			}
			*fixed_nodes = (int*)malloc(*n_nodes*sizeof(int));
		}
		else if(strcmp(query,"FIXED_NODES")==0)
		{
			char input[128];
			scanf("%s",input);
			int l = strlen(input);
			for (int i = 0; i < l; ++i)
			{
				if(input[i] == 'a'){
					(*fixed_nodes)[i] = 1;
				}
				else if(input[i] == 'i'){
					(*fixed_nodes)[i] = 0;
				}
				else if(input[i] == 'n'){
					(*fixed_nodes)[i] = -1;
				}
			}
		}
		else if(strcmp(query,"TOPOLOGY")==0)
		{
			for (int i = 0; i < *n_nodes; ++i)
			{
				for (int j = 0; j < *n_nodes; ++j)
				{
					scanf("%d",&((*topology)[i][j]));
				}
			}
		}
		else if(strcmp(query,"END")==0){
			break;
		}
	}

	parse_matrix(*n_nodes,*topology,strength);
}