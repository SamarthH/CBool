#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "parseMatrix.h"

void parse_matrix(int n, int** topo, char type)
{
	if (type=='n')// n implies normal
	{
		//Do Nothing
	}
	else if(type=='a')// a implies dominantly activating
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				if(topo[i][j]>0){
					topo[i][j] = 1<<5;
				}
			}
		}
	}
	else if(type =='i')// i implies dominantly inhibitary
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				if(topo[i][j]<0){
					topo[i][j] = -(1<<5);
				}
			}
		}
	}
	else{
		exit(2); // Error code 2 implies that incorrect type of nodes was chosen
	}
}