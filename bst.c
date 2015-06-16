#include "bst.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO:
// 	- Check heap allocations for `NULL` returns and handle it properly.
// 	- Check all arguments for `NULL`.


struct bst_t {
	node_t*		root;
	size_t		size;
	size_t		elem_size;
	bst_type_t	type;
	int		(*cmp)(const void*, const void*);
	void		(*data_free)(void*);
	void		(*print)(void*);
};

struct node_t {
	void*	data;
	node_t*	left;
	node_t*	right;
};

static node_t*	node_new (bst_t*, void* data);
static void	node_free (bst_t*, node_t*);

static void	bst_free_recursive	(bst_t*, node_t*);
static bool	bst_add_recursive	(bst_t*, node_t*, void* data);
node_t*		bst_delete_recursive	(bst_t*, node_t*, void* data);
static bool	bst_contains_recursive	(bst_t*, node_t*, void* data);
static size_t	bst_height_recursive	(bst_t*, node_t*);

static int	bst_to_array		(bst_t*, node_t*,
					 void* arr[], int index);

static node_t*	bst_build_tree		(bst_t*, void* arr[],
					 int first, int last);

static void	bst_print_recursive	(bst_t*, node_t*,
					 void (*print)(void*), int);


/*==============================================================================
	BINARY SEARCH TREE
==============================================================================*/

bst_t* bst_new(	bst_type_t	type,
		size_t		elem_size,
		int		(*cmp)(const void*, const void*),
		void		(*data_free)(void*),
		void		(*print)(void*))
{
	bst_t* bst = malloc(sizeof *bst);

	bst->root	= NULL;
	bst->size	= 0;
	bst->elem_size	= elem_size;
	bst->type	= type;
	bst->cmp	= cmp;
	bst->data_free	= data_free;
	bst->print	= print;

	return bst;
}

void bst_free(bst_t* bst)
{
	bst_free_recursive(bst, bst->root);
	free(bst);
}

static void bst_free_recursive(bst_t* bst, node_t* node)
{
	if (node == NULL) {
		return;
	}
	bst_free_recursive(bst, node->left);
	bst_free_recursive(bst, node->right);
	node_free(bst, node);
}

bool bst_add(bst_t* bst, void* data)
{
	if (bst->root == NULL) {
		bst->root = node_new(bst, data);
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
			node->left = node_new(bst, data);
			bst->size += 1;
			return true;
		} else {
			return bst_add_recursive(bst, node->left, data);
		}
	} else {
		if (node->right == NULL) {
			node->right = node_new(bst, data);
			bst->size += 1;
			return true;
		} else {
			return bst_add_recursive(bst, node->right, data);
		}
	}
}

static node_t* smallest_subnode(node_t* node)
{
	node_t* current = node;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

node_t* bst_delete(bst_t* bst, void* data)
{
	return bst_delete_recursive(bst, bst->root, data);
}

node_t* bst_delete_recursive(bst_t* bst, node_t* node, void* data)
{
	if (node == NULL) {
		return node;
	}

	int cmp_result = bst->cmp(data, node->data);

	if (cmp_result == 0) {
		if (node->left == NULL) {
			node_t* tmp = node->right;
			node_free(bst, node);
			return tmp;
		} else if (node->right == NULL) {
			node_t* tmp = node->left;
			node_free(bst, node);
			return tmp;
		}
		node_t* tmp = smallest_subnode(node->right);
		node->data = tmp->data; // TODO
		node->right = bst_delete_recursive(bst, node->right, tmp->data);
	} else if (cmp_result < 0) {
		node->left = bst_delete_recursive(bst, node->left, data);
	} else {
		node->right = bst_delete_recursive(bst, node->right, data);
	}
	return node;
}

bool bst_contains(bst_t* bst, void* data)
{
	if (bst == NULL || data == NULL) {
		printf("Error in bst_contains: NULL argument(s).\n");
		return false;
	}
	return bst_contains_recursive(bst, bst->root, data);
}

bool bst_contains_recursive(bst_t* bst, node_t* node, void* data)
{
	int cmp_result = bst->cmp(data, node->data);
	if (cmp_result == 0) {
		goto succ;
	} else if (cmp_result < 0) {
		if (node->left == NULL) {
			goto fail;
		} else {
			return bst_contains_recursive(bst, node->left, data);
		}
	} else {
		if (node->right == NULL) {
			goto fail;
		} else {
			return bst_contains_recursive(bst, node->right, data);
		}
	}
succ:	if (bst->print != NULL) {
		bst->print(data);
		printf(" exists in the tree.\n");
	}
	return true;
fail:	if (bst->print != NULL) {
		bst->print(data);
		printf(" does not exist in the tree.\n");
	}
	return false;
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
	if (node == NULL) {
		return;
	}
	execute(node->data);
	bst_execute_preorder_recursive(bst, node->left, execute);
	bst_execute_preorder_recursive(bst, node->right, execute);
}

static void
bst_execute_inorder_recursive(	bst_t*	bst,
				node_t*	node,
				void	(*execute)(void*))
{
	if (node == NULL) {
		return;
	}
	bst_execute_preorder_recursive(bst, node->left, execute);
	execute(node->data);
	bst_execute_preorder_recursive(bst, node->right, execute);
}

static void
bst_execute_postorder_recursive(bst_t*	bst,
				node_t*	node,
				void	(*execute)(void*))
{
	if (node == NULL) {
		return;
	}
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
	bst_t*	new_bst;

	last_index	= bst_to_array(bst, bst->root, arr, 0) - 1;

	new_bst		= bst_new(bst->type,
				  bst->elem_size,
				  bst->cmp,
				  bst->data_free,
				  bst->print);

	new_bst->root		= bst_build_tree(bst, arr, 0, last_index);
	new_bst->size		= bst->size;
	new_bst->cmp		= bst->cmp;
	new_bst->data_free	= bst->data_free;
	new_bst->print		= bst->print;

	return new_bst;
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
	index	   = bst_to_array(bst, node->right, arr, index + 1);
	return index;
}

static node_t* bst_build_tree(bst_t* bst, void* arr[], int first, int last)
{
	if (first > last) {
		return NULL;
	}
	int		mid;
	node_t*		mid_node;
	mid		= (first + last) / 2;
	mid_node	= node_new(bst, arr[mid]);
	mid_node->left	= bst_build_tree(bst, arr, first, mid - 1);
	mid_node->right	= bst_build_tree(bst, arr, mid + 1, last);
	return mid_node;
}

void bst_print(bst_t* bst, void (*print)(void* data))
{
	if (print == NULL) {
		printf("<No print function supplied.>");
		return;
	}
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

static node_t* node_new(bst_t* bst, void* data)
{
	node_t*	node = malloc(sizeof *node);

	switch (bst->type) {

	/* The BST makes a private copy of the data. */
	case BST_COPIED:
		node->data = malloc(bst->elem_size);
		memcpy(node->data, data, bst->elem_size);
		break;

	/* The BST does not take ownership of the data. */
	case BST_POINTED:
		node->data = data;
		break;

	default:
		printf("Invalid bst_type_t argument.\n");
		exit(1);
		break;
	}

	node->left	= NULL;
	node->right	= NULL;

	return node;
}

void node_free(bst_t* bst, node_t* node)
{
	if (node != NULL) {
		switch (bst->type) {
		case BST_COPIED:
			if (bst->data_free != NULL)
				bst->data_free(node->data);
			else
				// TODO warning
				;
		case BST_POINTED:			/* fall through */
			node->data = NULL;
			break;
		default:
			printf("Invalid bst_type_t argument.\n");
			exit(1);
			break;
		}
	}
}

#if 0
void node_free(node_t* node, void (*data_free)(void*))
{
	if (node != NULL) {
		if (data_free != NULL) {
			data_free(node->data);
		}
		free(node);
	}
}
#endif


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

