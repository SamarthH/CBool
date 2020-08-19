#ifndef ASYNC_UPDATE
#define ASYNC_UPDATE

#include "final_list.h"

asynclist* updateAsyncList(int n, int** topology, int* fixed_nodes, int height, int seed_init);
void updateAsyncTree(int n, int** topology, int* fixed_nodes, base* stable);
#endif