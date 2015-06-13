#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct bst_t bst_t;
typedef struct node_t node_t;

bst_t*	bst_new			(int (*cmp)(const void*, const void*));
void	bst_free		(bst_t* bst);
bool	bst_add			(bst_t* bst, void* data);
size_t	bst_size		(bst_t* bst);
size_t	bst_height		(bst_t* bst);
void	bst_print_inorder	(bst_t* bst, void (*print)(void* data));

int	cmp_int			(const void* a, const void* b);

#endif
