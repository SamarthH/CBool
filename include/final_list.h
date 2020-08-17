#ifndef FINAL_LIST
#define FINAL_LIST

typedef struct asynclistnode_t
{
	int n_occurances;
	int* state;
	struct asynclistnode_t* next;
	struct asynclistnode_t* prev;
}asynclistnode;

typedef struct synclistnode_t
{
	int n_occurances;
	int period;
	int** state_list;
	struct synclistnode_t* next;
	struct synclistnode_t* prev;
}synclistnode;

typedef struct asynclist_t
{
	asynclistnode* next;
}asynclist;

typedef struct synclist_t
{
	synclistnode* next;
}synclist;

void init_asynclist(asynclist* list);

void init_synclist(synclist* list);

void add_asynclist(asynclist* list, int* state, int n);

void add_synclist(asynclist* list, int** state_list, int n);

void clear_asynclist(asynclist* list);

void clear_synclist(synclist* list);

void print_asynclist(asynclist* list, int n);

void print_synclist(synclist* list, int n);

#endif