#include "final_list.h"
#include <stdio.h>
#include <stdlib.h>

asynclistnode* _search_async_state(asynclist* list, int* check_state, int n, int* flag)
{
	asynclistnode* node = list->next;
	if(!node){
		*flag = 0;
		return NULL;
	}
	*flag = 1;
	for (int i = 0; i < n && *flag; ++i)
	{
		if(node->state[i] != check_state[i]){
			*flag = 0;
		}
	}
	if(*flag == 1){
		return node;
	}
	while(node->next)
	{
		node = node->next;
		*flag = 1;
		for (int i = 0; i < n && *flag; ++i)
		{
			if(node->state[i] != check_state[i]){
				*flag = 0;
			}
		}
		if(*flag == 1){
			return node;
		}
	}
	return node;
}

void init_asynclist(asynclist* list)
{
	list->next = NULL;
}

void init_synclist(synclist* list)
{
	list->next = NULL;
}

void add_asynclist(asynclist* list, int* state, int n)
{
	int flag;
	asynclistnode* node = _search_async_state(list,state,n,&flag);
	if(flag){
		node->n_occurances++;
	}
	else if(!node)
	{
		asynclistnode* new_node = (asynclistnode*) malloc(sizeof(asynclistnode));
		new_node->state = (int*) malloc(n*sizeof(int));
		for (int i = 0; i < n; ++i)
		{
			new_node->state[i] = state[i];
		}
		new_node->n_occurances = 1;
		list->next = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
	}
	else{
		asynclistnode* new_node = (asynclistnode*) malloc(sizeof(asynclistnode));
		node->next = new_node;
		new_node->prev = node;
		new_node->next = NULL;
		new_node->state = (int*) malloc(n*sizeof(int));
		for (int i = 0; i < n; ++i)
		{
			new_node->state[i] = state[i];
		}
		new_node->n_occurances = 1;
	}
}

void clear_asynclist(asynclist* list)
{
	asynclistnode* node = list->next;
	while(node)
	{
		asynclistnode* nextnode = node->next;
		free(node->state);
		free(node);
		node = nextnode;
	}
	free(list);
}

void print_asynclist(asynclist* list, int n)
{
	printf("N_Occurances State\n");
	asynclistnode* node = list->next;
	while(node)
	{
		printf("%d",node->n_occurances);
		for (int i = 0; i < n; ++i)
		{
			printf(" %d",node->state[i]);
		}
		printf("\n");
		node = node->next;
	}
}