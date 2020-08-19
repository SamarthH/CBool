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
	list->n_elements = 0;
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
		list->n_elements++;
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
		list->n_elements++;
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

asynclist* merge_asynclist(asynclist* list1, asynclist* list2, int n)
{
	if(list1->n_elements<list2->n_elements)
	{
		asynclist* temp = list1;
		list1 = list2;
		list2 = temp;
	}

	asynclistnode* node = list2->next;
	while(node)
	{
		asynclistnode* check = list1->next;
		int flag;
		while(1)
		{
			flag = 1;
			for (int i = 0; i < n && flag; ++i)
			{
				if(node->state[i] != check->state[i])
				{
					flag = 0;
				}
			}

			if(flag){
				flag = 1;
				break;
			}
			if(!(check->next))
			{
				break;
			}
			else{
				check = check->next;
			}
		}
		if(flag)
		{
			check->n_occurances += node->n_occurances;
		}
		else
		{
			asynclistnode* new = (asynclistnode*)malloc(sizeof(asynclistnode));
			check->next = new;
			new->next = NULL;
			new->prev = check;
			new->n_occurances = node->n_occurances;
			new->state = (int*)malloc(n*sizeof(int));
			for (int i = 0; i < n; ++i)
			{
				new->state[i] = node->state[i];
			}
			list1->n_elements++;
		}
		node = node->next;
	}
	clear_asynclist(list2);
	return list1;
}

void init_tree(base* t)
{
	t->root = NULL;
}

int _compare(int n, int* a, int* b)
{
	for (int i = 0; i < n; ++i)
	{
		if(a[i]>b[i])
		{
			return -1;
		}
		else if(a[i]<b[i])
		{
			return 1;
		}
	}
	return 0;
}

void add_node(base* t, int n, int* state)
{
	tree* node = t->root;
	if(!node)
	{
		tree* newnode = (tree*)malloc(sizeof(tree));
		newnode->state = (int*)malloc(n*sizeof(int));
		newnode->n_occurances = 1;
		#pragma omp parallel for
		for (int i = 0; i < n; ++i)
		{
			newnode->state[i] = state[i];
		}
		t->root = newnode;
		newnode->left = newnode->right = NULL;
		return ;
	}
	while(node)
	{
		int comp = _compare(n,node->state,state);
		if(!comp)
		{
			node->n_occurances++;
			return;
		}
		else if(comp == 1)
		{
			if(node->right){
				node = node->right;
			}
			else{
				tree* newnode = (tree*)malloc(sizeof(tree));
				newnode->state = (int*)malloc(n*sizeof(int));
				newnode->n_occurances = 1;
				#pragma omp parallel for
				for (int i = 0; i < n; ++i)
				{
					newnode->state[i] = state[i];
				}
				node->right = newnode;
				newnode->left = newnode->right = NULL;
				return;
			}
		}
		else{
			if(node->left){
				node = node->left;
			}
			else{
				tree* newnode = (tree*)malloc(sizeof(tree));
				newnode->state = (int*)malloc(n*sizeof(int));
				newnode->n_occurances = 1;
				#pragma omp parallel for
				for (int i = 0; i < n; ++i)
				{
					newnode->state[i] = state[i];
				}
				node->left = newnode;
				newnode->left = newnode->right = NULL;
				return;
			}
		}
	}
}

void _delete_node(tree* t)
{
	if(t==NULL)
	{
		return;
	}
	else{
		tree* child[2] = {t->left,t->right};
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			_delete_node(child[i]);
		}
		free(t->state);
		free(t);
	}
}

void delete_tree(base* t)
{
	_delete_node(t->root);
}

void _print_recur(tree* t, int n)
{
	if(t==NULL)
	{
		return;
	}
	else{
		tree* child[2] = {t->left,t->right};
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			_print_recur(child[i],n);
		}
		#pragma omp critical(printing)
		{
			printf("%d\t", t->n_occurances);
			for (int i = 0; i < n; ++i)
			{
				printf("%d ",t->state[i]);
			}
			printf("\n");
		}
	}
}

void print_tree(base* t, int n)
{
	_print_recur(t->root,n);
}