#include "bst.h"
#include <stdio.h>

void print_int(void* data)
{
	printf("<%d> ", *((int*)data));
}

int main()
{
	bst_t*	bst;
	int	arr[] = { 5, 2, 3, 9, 6, 7, 1, 8, 4, 10 };

	bst = bst_new(cmp_int);
	for (int i = 0; i < 10; ++i) {
		bst_add(bst, &arr[i]);
	}
	bst_print_inorder(bst, print_int);
	printf("height = %zu\n", bst_height(bst));

	bst_free(bst);
}
