#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct bst_t bst_t;
typedef struct node_t node_t;

typedef enum {
	ORDER_PRE,
	ORDER_IN,
	ORDER_POST,
} traversal_order_t;

/* Create a new BST (Binary Search Tree). The argument `cmp` is a pointer to
 * a function that compares two instances of data of the same type, and returns:
 * 	== 0,	if the arguments are equal
 * 	 > 0,	if the first argument is greater than the second
 * 	 < 0,	if the first argument is smaller than the second */
bst_t*	bst_new		(int (*cmp)(const void*, const void*));

/* Free all the nodes in the BST pointer to by `bst`, and thereafter free `bst`.
 * `data_free` is a pointer to a function that frees data of the type contained
 * in the node. */
void	bst_free	(bst_t* bst, void (*data_free)(void* data));

/* First, the function checks whether or not `data` is contained in the BST.
 * This test is done by the compare function passed to the BST as it was created
 * (with bst_new). If `data` already exists in the BST, nothing happens.
 * Otherwise a new node is created in `bst` and its data field is pointer to
 * `data`. */
bool	bst_add		(bst_t* bst, void* data);

/* Return the number of nodes in `bst`. The lookup is performed in O(1) time. */
size_t	bst_size	(bst_t* bst);

/* Recursively return the height (or depth) of `bst`. */
size_t	bst_height	(bst_t* bst);

/* Call the function pointed to by `execute` on the data pointer to by `data`
 * for every node in `bst` in the order specified by `order`. */
void	bst_execute	(bst_t*			bst,
			 void			(*execute)(void* data),
			 traversal_order_t	order);

void	bst_print	(bst_t* bst, void (*print)(void* data));

#endif
