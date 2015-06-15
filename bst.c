#include "bst.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO
// 	Look for all the TODOs in this file.
//

struct bst_t {
	node_t*	root;
	size_t	size;
	size_t	elem_size;
	int	(*cmp)(const void*, const void*);
	void	(*data_free)(void*);
	void	(*print)(void*);
};

struct node_t {
	void*	data;
	node_t*	left;
	node_t*	right;
};

static node_t*	node_new (void* data);
static void	node_free (node_t*, void (*data_free)(void*));


static void	bst_free_recursive (bst_t*, node_t*);
static bool	bst_add_recursive (bst_t*, node_t*, void* data);
static size_t	bst_height_recursive (bst_t*, node_t*);
static int	bst_to_array (bst_t* bst, node_t* node, void* arr[], int index);
static node_t*	bst_build_tree (void* arr[], int first, int last);
static void	bst_print_recursive(bst_t*, node_t*, void (*print)(void*), int);


/*==============================================================================
	BINARY SEARCH TREE
==============================================================================*/

bst_t* bst_new(	size_t	elem_size,
		int	(*cmp)(const void*, const void*),
		void	(*data_free)(void*),
		void	(*print)(void*))
{
	bst_t* bst = malloc(sizeof *bst);

	bst->root	= NULL;
	bst->size	= 0;
	bst->elem_size	= elem_size;
	bst->cmp	= cmp;
	bst->data_free	= data_free;
	bst->print	= print;

	return bst;
}

void bst_free(bst_t* bst)
{
	printf("\nFreeing ...\n");
	bst_free_recursive(bst, bst->root);
	printf("Done!\n");
	free(bst);
}

static void bst_free_recursive(bst_t* bst, node_t* node)
{
	if (node == NULL)
		return;
	bst_free_recursive(bst, node->left);
	bst_free_recursive(bst, node->right);
	node_free(node, bst->data_free);
}

bool bst_add(bst_t* bst, void* data)
{
	if (bst->root == NULL) {
		bst->root = node_new(data);
		bst->size += 1;
		return true;
	}
	return bst_add_recursive(bst, bst->root, data);
}

static bool bst_add_recursive(bst_t* bst, node_t* node, void* data)
{
	int cmp_result = bst->cmp(data, node->data);
	if (cmp_result == 0) {	/* Base case */
		printf("Node already exists inside the BST. Doing nothing.\n");
		return false;
	} else if (cmp_result < 0) {
		if (node->left == NULL) {
			node->left = node_new(data);
			bst->size += 1;
			return true;
		} else {
			return bst_add_recursive(bst, node->left, data);
		}
	} else {
		if (node->right == NULL) {
			node->right = node_new(data);
			bst->size += 1;
			return true;
		} else {
			return bst_add_recursive(bst, node->right, data);
		}
	}
}

inline size_t bst_size(bst_t* bst)
{
	return bst->size;
}

size_t bst_height(bst_t* bst)
{
	return bst_height_recursive(bst, bst->root);
}

static inline int max(const int a, const int b)
{
	return a > b ? a : b;
}

static size_t bst_height_recursive(bst_t* bst, node_t* node)
{
	if (node == NULL) {
		return 0;
	} else {
		return 1 + max(	bst_height_recursive(bst, node->left),
				bst_height_recursive(bst, node->right));
	}
}

static void
bst_execute_preorder_recursive  (bst_t*, node_t*, void (*execute)(void*));

static void
bst_execute_inorder_recursive   (bst_t*, node_t*, void (*execute)(void*));

static void
bst_execute_postorder_recursive (bst_t*, node_t*, void (*execute)(void*));

#define BST_EXECUTE(ORDER) bst_execute_ ## ORDER ## _recursive

void bst_execute(bst_t*			bst,
		 void			(*execute)(void* data),
		 traversal_order_t	order)
{
	switch (order) {
	case ORDER_PRE:	 BST_EXECUTE(preorder)	(bst, bst->root, execute);break;
	case ORDER_IN:	 BST_EXECUTE(inorder)	(bst, bst->root, execute);break;
	case ORDER_POST: BST_EXECUTE(postorder)	(bst, bst->root, execute);break;
	}
}

static void
bst_execute_preorder_recursive(	bst_t*	bst,
				node_t* node,
				void	(*execute)(void*))
{
	if (node == NULL)
		return;
	execute(node->data);
	bst_execute_preorder_recursive(bst, node->left, execute);
	bst_execute_preorder_recursive(bst, node->right, execute);
}

static void
bst_execute_inorder_recursive(	bst_t*	bst,
				node_t*	node,
				void	(*execute)(void*))
{
	if (node == NULL)
		return;
	bst_execute_preorder_recursive(bst, node->left, execute);
	execute(node->data);
	bst_execute_preorder_recursive(bst, node->right, execute);
}

static void
bst_execute_postorder_recursive(bst_t*	bst,
				node_t*	node,
				void	(*execute)(void*))
{
	if (node == NULL)
		return;
	bst_execute_preorder_recursive(bst, node->left, execute);
	bst_execute_preorder_recursive(bst, node->right, execute);
	execute(node->data);
}

bst_t* bst_balanced(bst_t* bst)
{
	if (bst == NULL || bst->root == NULL) {
		printf("Nothing to balance.\n");
		return NULL;
	}
	void*	arr[bst->size];
	int	last_index;
	bst_t*	new;

	last_index	= bst_to_array(bst, bst->root, arr, 0) - 1;

	new		= bst_new(bst->elem_size, bst->cmp,
				  bst->data_free, bst->print);
	new->root	= bst_build_tree(arr, 0, last_index);
	new->size	= bst->size;
	new->cmp	= bst->cmp;
	new->data_free	= bst->data_free;
	new->print	= bst->print;
	// TODO: use memcpy instead for a (very) slight speedup (?)

//	bst_print(bst, bst->print);

	return new;
}

static int
bst_to_array(	bst_t*	bst,
		node_t*	node,
		void*	arr[],
		int	index)
{
	if (node == NULL) {
		return index;
	}
	index	   = bst_to_array(bst, node->left, arr, index);
	arr[index] = node->data;
//	memcpy(arr[index], node->data, bst->elem_size);
	index	   = bst_to_array(bst, node->right, arr, index + 1);
	return index;
}

static node_t* bst_build_tree(void* arr[], int first, int last)
{
	if (first > last) {
		return NULL;
	}
	int		mid;
	node_t*		mid_node;
	mid		= (first + last) / 2;
	//
	// TODO:
	// 	If a new tree is build and the old one freed, the data
	// 	in the new tree will also be freed because as it currently
	// 	is, node_new(...) will point the data in the new tree to
	// 	the data in arr[...], which in its turn points to the data
	// 	in the old tree. ___`arr` is just an array of pointers!___
	//
	// 	To counter this issue, some kind of flag or indicator should
	// 	be passed when creating the tree that tells the program
	// 	if the data need to be freed or not.
	//
	// 	See the todo inside the `node_new` function.
	//

	mid_node	= node_new(arr[mid]);
	mid_node->left	= bst_build_tree(arr, first, mid - 1);
	mid_node->right	= bst_build_tree(arr, mid + 1, last);
	return mid_node;
}

void bst_print(bst_t* bst, void (*print)(void* data))
{
	printf("Printing the BST:\n");
	bst_print_recursive(bst, bst->root, print, 0);
}

static void
bst_print_recursive(bst_t*	bst,
		    node_t*	node,
		    void	(*print)(void* data),
		    int		level)
{
	printf("  ");
	for (int i = 0; i < level; ++i) {
		printf("%s", i == 0 ? "|——" : "———");
	}
	if (node == NULL) {
		printf("( )\n");
		return;
	}
	printf("(");
	print(node->data);
	printf(")\n");
	bst_print_recursive(bst, node->left, print, level + 1);
	bst_print_recursive(bst, node->right, print, level + 1);
}



/*==============================================================================
	NODE
==============================================================================*/

static node_t* node_new(void* data)
{
	node_t*	node = malloc(sizeof *node);

	//
	// TODO
	// 	See the todo inside the `bst_build_tree` function. If the user
	// 	asks the BST to manage the memory (this request should only be
	// 	present if the user passes pointers to blocks of heap-allocated
	// 	memory), then the assignment
	//
	// 		node->data = data
	//
	// 	should be changed to something like
	//
	// 		memcpy(node->data, data, elem_size)
	//
	// 	(pass `elem_size` to `node_new`). This makes freeing the old
	// 	BST safe by avoiding freeing memory that was already freed.
	//
	/* The BST does not take ownership of the data. */
	node->data	= data;
	node->left	= NULL;
	node->right	= NULL;

	return node;
}

void node_free(node_t* node, void (*data_free)(void*))
{
	if (node != NULL) {
		data_free(node->data);
		free(node);
	}
}



//
// TODO (Possible implementation):
// 	Eventually I might make a function that takes a pointer to a pointer
// 	to a `struct bst` and balances it in place. I imagine it would be
// 	useful if one does not want to keep the old unbalanced tree (it would
// 	result in less boilerplate API).
//
// 	The function declaration would look like the following:
//
//		void bst_balance(bst_t** bst);
//
//	and its implementation would be very similar (algorithmically)
//	to the existing `bst_balanced` function.
//

#if 0
void bst_balance(bst_t** bst, size_t elem_size)
{
	// Implement!
}
#endif

