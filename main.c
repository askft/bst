#include "bst.h"
#include <stdio.h>
#include <string.h>

/* Add person to the BST, print them and then free the nodes */

typedef struct {
	char	name[128];
	int	age;
} person_t;

person_t* person_new(const char* name, int age)
{
	person_t* p = malloc(sizeof(*p));
	strcpy(p->name, name);
	p->age = age;
	return p;
}

void person_print(void* person)
{
	person_t* p = (person_t*) person;

	printf("{ name = \"%s\", age = %d }", p->name, p->age);
}

void person_free(void* data)
{
	person_t* p = (person_t*) data;
	person_print(p);
	free(p);
}

/* If two persons have the same name and age, they are considered to be the
 * same person. */
int person_cmp(const void* a, const void* b)
{
	person_t*	pa		= (person_t*) a;
	person_t*	pb		= (person_t*) b;
	int		res_name	= strcmp(pa->name, pb->name);

	if (res_name == 0)
		return pa->age - pb->age;
	else
		return res_name;
}

#if 0
int main(void)
{
	bst_t* bst	= bst_new(person_cmp);

	bst_add		(bst, person_new("Alexander", 20));
	bst_add		(bst, person_new("Alexander", 20));
	bst_add		(bst, person_new("Alexander", 25));
	bst_add		(bst, person_new("Johnny Bravo", 16));
	bst_add		(bst, person_new("Knugen", 37));
	bst_add		(bst, person_new("N.C. Overguard", 37));
//	bst_execute	(bst, person_print, ORDER_PRE);
	bst_print	(bst, person_print);
	bst_free	(bst, person_free);
}
#endif



int int_cmp(const void* a, const void* b)
{
	return *((int*)a) - *((int*)b);
}

void int_print(void* data)
{
	printf("%d", *((int*)data));
}

void int_free(void* data)
{
	printf("(%d)", *((int*)data));
}

int main()
{
	bst_t*	bst;
	int	arr[] = { 5, 2, 3, 9, 6, 7, 1, 8, 4, 10 };

	bst = bst_new(int_cmp);
	for (int i = 0; i < 10; ++i) {
		bst_add(bst, &arr[i]);
	}

	bst_print(bst, int_print);
	printf("height = %zu\n", bst_height(bst));

	bst_free(bst, int_free);
}

