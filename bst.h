#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdlib.h>


typedef struct bst_t bst_t;
typedef struct node_t node_t;


/*==============================================================================
 * Create a new BST (Binary Search Tree).
 *
 * @arg `elem_size`
 * 	The size of the elements that will be stored inside the BST. If for
 * 	instance `struct X` is the data type that will be stored, then
 * 	`sizeof(struct X)` shall be passed as this argument.
 *
 * @arg	`cmp`
 * 	A pointer to a function that compares two instances of data of the same
 * 	type, and returns:
 * 		== 0,	if the arguments are equal
 * 		 > 0,	if the first argument is greater than the second
 * 		 < 0,	if the first argument is smaller than the second
 *
 * @arg `data_free`
 * 	A pointer to a function that frees data of the type contained in the
 * 	node (i.e. the type of which pointers to are added as the `data`
 * 	argument of bst_add).
 *
 * @arg `print`
 * 	A pointer to a function that prints the data contained in a BST node.
 *
 * @return
 * 	A pointer to a bst_t struct. The caller need not know anything about
 * 	this struct; it is just handle that should be passed to the remaining
 * 	bst_? functions.
 */
bst_t*	bst_new		(size_t	elem_size,
			 int	(*cmp)(const void*, const void*),
			 void	(*data_free)(void*),
			 void	(*print)(void*));


/*==============================================================================
 * Deallocate memory used by the BST.
 *
 * Free all the nodes in the BST pointed to by `bst`, and thereafter free
 * `bst` itself. The data contained in the nodes is not freed; the *caller*
 * thus has responsibility for the lifetime of the data of the BST.
 */
void	bst_free	(bst_t* bst);


/*==============================================================================
 * Add a pointer to some type of data to the BST. The data is not copied nor is
 * new memory allocated for it.
 *
 * First, the function checks whether or not `data` is contained in the BST.
 * This test is done by the compare function passed to the BST as it was created
 * (with bst_new). If `data` already exists in the BST, nothing happens.
 * Otherwise a new node is created in `bst` and its data field is pointer to
 * `data`.
 *
 * Directly passing a pointer to block of memory to `data` without keeping a
 * reference to it may cause a memory leak if the same data is added twice.
 */
bool	bst_add		(bst_t* bst, void* data);


/*==============================================================================
 * Return the number of nodes in `bst`. The lookup is performed in O(1) time. */
size_t	bst_size	(bst_t* bst);


/*==============================================================================
 * Recursively return the height (or depth) of `bst`.
 * */
size_t	bst_height	(bst_t* bst);


/*==============================================================================
 * The order in which the BST shall be traversed. Pass one of these as an
 * argument to the `bst_execute` function declared below.
 */
typedef enum { ORDER_PRE, ORDER_IN, ORDER_POST, } traversal_order_t;


/*==============================================================================
 * Call the function pointed to by `execute` on the data pointed to by `data`
 * for every node in `bst` in the order specified by `order`.
 */
void	bst_execute	(bst_t*			bst,
			 void			(*execute)(void* data),
			 traversal_order_t	order);


/*==============================================================================
 * Balance the BST.
 *
 * @return
 * 	A pointer to a new BST. The old BST has to be freed by the caller by
 * 	calling the `bst_free` function declared in this file.
 */
bst_t*	bst_balanced	(bst_t* bst);


/*==============================================================================
 * Print a representation of the BST to `stdout`.
 */
void	bst_print	(bst_t* bst, void (*print)(void* data));


#endif
