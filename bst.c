#include "bst.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bst_t {
	node_t*	root;
	size_t	size;
	int	(*cmp)(const void*, const void*);
};

struct node_t {
	void*	data;
	node_t*	left;
	node_t*	right;
};

static node_t*	node_new (void* data);
static void	node_free (node_t*, void (*data_free)(void*));

static void	bst_free_recursive (bst_t*, node_t*, void (*data_free)(void*));
static bool	bst_add_recursive (bst_t*, node_t*, void* data);
static size_t	bst_height_recursive (bst_t*, node_t*);
static void	bst_print_recursive(bst_t*, node_t*, void (*print)(void*), int);



/*==============================================================================
	BINARY SEARCH TREE
==============================================================================*/

bst_t* bst_new(int (*cmp)(const void*, const void*))
{
	bst_t* bst = malloc(sizeof *bst);

	bst->root	= NULL;
	bst->size	= 0;
	bst->cmp	= cmp;

	return bst;
}

void bst_free(bst_t* bst, void (*data_free)(void*))
{
	printf("Freeing:\n");
	bst_free_recursive(bst, bst->root, data_free);
	printf("\n... done.\n\n");
	free(bst);
}

static void bst_free_recursive(	bst_t*	bst,
				node_t*	node,
				void	(*data_free)(void*))
{
	if (node == NULL)
		return;
	bst_free_recursive(bst, node->left, data_free);
	bst_free_recursive(bst, node->right, data_free);
	node_free(node, data_free);
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

size_t bst_size(bst_t* bst)
{
	return bst->size;
}

size_t bst_height(bst_t* bst)
{
	return bst_height_recursive(bst, bst->root);
}

#define MAX(A, B) ((A) > (B) ? (A) : (B))

static size_t bst_height_recursive(bst_t* bst, node_t* node)
{
	if (node == NULL) {
		return 0;
	} else {
		return 1 + MAX(	bst_height_recursive(bst, node->left),
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

void bst_print(bst_t* bst, void (*print)(void* data))
{
	printf("Printing the BST:\n--------------------\n");
	bst_print_recursive(bst, bst->root, print, 0);
	printf("\n--------------------\n\n");
}

static void
bst_print_recursive(bst_t*	bst,
		    node_t*	node,
		    void	(*print)(void* data),
		    int		level)
{
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
	
	node->data	= data;
	node->left	= NULL;
	node->right	= NULL;

	return node;
}

void node_free(node_t* node, void (*data_free)(void*))
{
	if (node != NULL) {
		printf(" ");
		data_free(node->data);
		free(node);
	}
}



#if 0
static inline const void* greater(bst_t* bst, const void* a, const void* b)
{
	return bst->cmp(a, b) > 0 ? a : b;
}
#endif

