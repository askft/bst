#include "bst.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bst_t {
	node_t*	root;
	size_t	size;					/* Number of nodes */
	size_t	elem_size;				/* Size of a node */
	int	(*cmp)(const void*, const void*);	/* Compare function */
};

struct node_t {
	void*	data;
	node_t*	left;
	node_t*	right;
};

static node_t*	node_new (void* data);
static void	node_free (node_t* node);

static void	bst_free_recursive (bst_t* bst, node_t* node);
static bool	bst_add_recursive (bst_t* bst, node_t* node, void* data);
static void	bst_print_inorder_recursive (bst_t*	bst,
					     node_t*	node,
					     void	(*print)(void* data));
static size_t	bst_height_recursive (bst_t* bst, node_t* node);

int cmp_int(const void* a, const void* b)
{
	return *((int*) a) - *((int*) b);
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

static void node_free(node_t* node)
{
	if (node != NULL) {
		printf("<%d> ", *((int*)node->data));
		free(node);
	}
}



/*==============================================================================
	BINARY SEARCH TREE
==============================================================================*/

bst_t* bst_new(int (*cmp)(const void*, const void*))
{
	bst_t* bst = malloc(sizeof *bst);

	bst->root	= NULL;
	bst->size	= 0;
	bst->elem_size	= 0;
	bst->cmp	= cmp;

	return bst;
}

// TODO:
// 	Perhaps this function should take a function pointer to a node_free
// 	function.
void bst_free(bst_t* bst)
{
	printf("Freeing: ");
	bst_free_recursive(bst, bst->root);
	printf("... done.\n");
	free(bst);
}

static void bst_free_recursive(bst_t* bst, node_t* node)
{
	if (node == NULL)
		return;

	bst_free_recursive(bst, node->left);
	bst_free_recursive(bst, node->right);

	node_free(node);
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

void bst_print_inorder(bst_t* bst, void (*print)(void* data))
{
	bst_print_inorder_recursive(bst, bst->root, print);
	printf("\n");
}

static void bst_print_inorder_recursive(bst_t*	bst,
					node_t*	node,
					void	(*print)(void* data))
{
	if (node == NULL)
		return;
	bst_print_inorder_recursive(bst, node->left, print);
	print(node->data);
	bst_print_inorder_recursive(bst, node->right, print);
}

