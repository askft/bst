#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bst_t bst_t;

struct bst_t {
	void*	data;
	bst_t*	left;
	bst_t*	right;
};

bst_t* bst_create()
{
	bst_t*	bst;

	bst = malloc(sizeof *bst);

	if (bst == NULL) {
		fprintf(stderr, "[!] malloc failed.\n");
		return NULL;
	}

	bst->data	= NULL;
	bst->left	= NULL;
	bst->right	= NULL;

	return bst;
}

int bst_add(bst_t*	bst,
	    void*	data,
	    size_t	elem_size,
	    int		(*cmp)(const void* p1, const void* p2))
{
	int cmp_res;

	if (bst == NULL) {
		printf("ERROR\n");
		exit(1);
	}

	bst->data = malloc(elem_size);	

	if (bst->data == NULL) {
		fprintf(stderr, "[!] malloc failed.\n");
		return;
	}

	cmp_res = cmp(bst->data, data);
	if (cmp_res < 0) {
		bst->left = malloc(elem_size); // TODO: check
		memcpy(bst->left->data, data, sizeof(data));
	} else if (cmp_res > 0) {

	} else {
		// TODO: Do nothing
	}

	memcpy(bst->data, data, sizeof(data));


}

int bst_is_leaf(bst_t* bst)
{
	if (bst == NULL) {
		fprintf(stderr, "[!] Can't check if NULL is a leaf!\n");
		return 0;
	}
	return bst->left == NULL && bst->right == NULL;
}

int cmp_int(const void* a, const void* b)
{
	return *((int*) a) - *((int*) b);
}

int main()
{
	bst_t*	bst;

	int arr[] = {1,2,3,4,5};

	bst = bst_create();
	for (int i = 0; i < 5; ++i) {
		bst_add(bst, &arr[i], sizeof(int), cmp_int);
	}
}

