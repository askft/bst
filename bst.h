#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct bst_t bst_t;
typedef struct node_t node_t;

/*==============================================================================
 * This enum decides, when passed as an argument to `bst_new`, how data will be
 * handled. Please read the documentation for `bst_new` for a detailed
 * description of the intended usage of this enum.
 */
typedef enum { BST_COPIED, BST_POINTED, } bst_type_t;


/*==============================================================================
 * Create a new BST (Binary Search Tree).
 *
 * @arg `type`
 * 	An enum dictating how the BST will handle the data that is passed to
 * 	it when adding nodes.
 *
 * 		- BST_COPIED:
 * 			The BST will allocate new memory on the heap for the
 * 			data. This data does not have to be explicitly freed by
 * 			the caller; HOWEVER, he or she has to write a custom
 * 			free-function passed to this functions as the
 * 			`data_free` argument. For simple stack-allocated data
 * 			it would just something like:
 *
 * 				void free_stuff(void* data)
 * 				{
 * 					free(data);
 * 				}
 *
 * 			Using BST_COPIED implicitly results in the BST being
 * 			able to have a lifetime of desired length, since the
 * 			data in the nodes is not lost when the original data
 * 			that was passed as arguments to the `bst_add` function
 * 			goes out of scope.
 *
 * 		- BST_POINTED:
 * 			The BST will only point the data part of each node to
 * 			the data passed to the `bst_add` function. The function
 * 			pointer to free memory, mentioned above, may be replaced
 * 			by NULL as the argument. Using BST_POINTED will
 * 			implicitly result in the BST having a shorter or equally
 * 			long lifetime as the data it holds.
 *
 * 	When passing heap-allocated data (which the caller is responsible for
 * 	freeing) to `bst_new`, you should always use BST_COPIED. When passing
 * 	stack-allocated (automatically deallocated) data, it does not matter.
 *
 * @arg `elem_size`
 * 	The size of the elements that will be stored inside the BST. If for
 * 	instance `struct X` is the data type that will be stored, then
 * 	`sizeof(struct X)` shall be passed as this argument.
 *
 * @arg	`cmp`
 * 	A pointer to a function that compares two instances of data of the same
 * 	type, and returns an integer which is
 * 		== 0,	if the arguments are equal
 * 		 > 0,	if the first argument is greater than the second
 * 		 < 0,	if the first argument is smaller than the second
 *
 * @arg `data_free`
 * 	A pointer to a function that frees data of the type contained in the
 * 	node (i.e. the type of which pointers to are added as the `data`
 * 	argument of bst_add). Pass `NULL` if you do not want the tree to
 * 	free any data. Keep in mind though that if BST_COPIED is passed as
 * 	`type`, passing `NULL` as `data_free` will result in a memory
 * 	leak.
 *
 * @arg `print`
 * 	A pointer to a function that prints the data contained in a BST node.
 * 	Passing `NULL` will print a short message saying that there is no print
 * 	function available.
 *
 * @return
 * 	A pointer to a bst_t struct. The caller need not know anything about
 * 	this struct; it is just a handle that should be passed to the remaining
 * 	bst_? functions.
 */
bst_t*	bst_new		(bst_type_t	type,
			 size_t		elem_size,
			 int		(*cmp)(const void*, const void*),
			 void		(*data_free)(void*),
			 void		(*print)(void*));


/*==============================================================================
 * Deallocate memory used by the BST.
 *
 * Free all the nodes in the BST pointed to by `bst`, and thereafter free
 * `bst` itself.
 *
 * The data contained in the nodes is freed with the `data_free` function passed
 * to the BST when it is created. If this function is `NULL`, the data inside
 * the nodes will not be freed.
 */
void	bst_free	(bst_t* bst);


/*==============================================================================
 * Add a pointer to some type of data to the BST. If BST_COPIED was used when
 * creating the BST, new memory will be allocated. If BST_POINTED was used, then
 * the data pointer inside each node will only be pointed to the data passed
 * to this function.
 *
 * First, the function checks whether or not `data` is contained in the BST.
 * This test is done by the compare function passed to the BST as it was created
 * (with bst_new). If `data` already exists in the BST, nothing happens.
 * Otherwise a new node is created in `bst`.
 *
 * Directly passing a pointer to block of memory to `data` without keeping a
 * reference to it may cause a memory leak if the same data is added twice.
 */
bool	bst_add		(bst_t* bst, void* data);


/*==============================================================================
 * If found, delete the node containing `data` and return its value, otherwise
 * return `NULL`.
 */
node_t*	bst_delete	(bst_t* bst, void* data);


/*==============================================================================
 * Return true if the BST contains `data`.
 */
bool	bst_contains	(bst_t* bst, void* data);


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
 * for every node in `bst` in the order specified by `order`. For instance,
 * the function passed as `execute` could be:
 * 
 * 	void print_address(void* data)
 * 	{
 *		printf("%p\n", data);
 * 	}
 *
 * ... and then used as `bst_execute(bst, print_address, ORDER_IN);`.
 */
void	bst_execute	(bst_t*			bst,
			 void			(*execute)(void* data),
			 traversal_order_t	order);


/*==============================================================================
 * Balance the BST.
 *
 * @return
 * 	A pointer to a new BST. The old BST has to be freed by the caller by
 * 	calling the `bst_free` function declared in this file (if the caller
 * 	so wishes).
 */
bst_t*	bst_balanced	(bst_t* bst);


/*==============================================================================
 * Print a representation of the BST to `stdout`. The `print` function pointer
 * is is of the same kind as the one used when creating the tree. The reason
 * for explicitly passing `print` here is that the caller may want to print
 * data differently depending on the situation.
 */
void	bst_print	(bst_t* bst, void (*print)(void* data));


#endif
