#include "bst.h"
#include <stdio.h>
#include <string.h>

typedef struct {
	char	name[128];
	int	age;
} person_t;

void test_person_heap	(void);
void test_person	(void);
void test_int		(void);

person_t*	person_new_heap	(const char* name, int age);
person_t 	person_new_stack(const char* name, int age);
void		person_free_heap(void* data);
int		person_cmp	(const void* a, const void* b);
void		person_print	(void* data);

int		int_cmp		(const void* a, const void* b);
void		int_print	(void* data);

int main(void)
{
	test_int	();
	test_person	();
	test_person_heap();
}

void test_int()
{
	printf( "----------------------------------------\n"
		" test_int\n"
		"----------------------------------------\n\n" );
	bst_t*	bst;
	int	arr[10];

	bst = bst_new(BST_COPIED, sizeof(int), int_cmp, free, int_print);
	if (bst == NULL) {
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 10; ++i) {
		arr[i] = i + 1;
		bst_add(bst, &arr[i]);
	}

	bst_print	(bst, int_print);

	printf		("\nBalancing the tree...\n");
	bst_t* tmp	= bst;
	bst		= bst_balanced(tmp);
	bst_free	(tmp);
	printf		("Balanced the tree!\n\n");

	bst_print	(bst, int_print);
	bst_delete	(bst, &arr[5 - 1]);
	bst_print	(bst, int_print);

	bst_free	(bst);

	printf("\n\n");
}

void test_person()
{
	printf( "----------------------------------------\n"
		" test_person\n"
		"----------------------------------------\n\n" );

	bst_t* bst	= bst_new(BST_POINTED,
				  sizeof(person_t),
				  person_cmp, NULL, person_print);
	bst_t* tmp	= bst;

	person_t persons[] = {
		person_new_stack("Alexander", 20),
		person_new_stack("Donald Knuth", 25),
		person_new_stack("Johnny Bravo", 16),
		person_new_stack("Knugen", 37),
		person_new_stack("N.C. Overguard", 37),
	};

	int n = sizeof(persons) / sizeof(persons[0]);

	for (int i = 0; i < n; ++i) {
		bst_add(bst, &persons[i]);
	}

	bst_print(bst, person_print);

	bst = bst_balanced(tmp);

	bst_free	(tmp);
	bst_print	(bst, person_print);

	bst_contains	(bst, &persons[0]);
	bst_delete	(bst, &persons[0]);
	bst_contains	(bst, &persons[0]);
	bst_print	(bst, person_print);

	bst_free	(bst);

	printf("\n\n");
}

void test_person_heap()
{
	printf( "----------------------------------------\n"
		" test_person alloc\n"
		"----------------------------------------\n\n" );

	bst_t* bst	= bst_new(BST_COPIED, sizeof(person_t), person_cmp,
				  person_free_heap, person_print);
	bst_t* tmp	= bst;

	/* Allocate some memory on the heap and store pointers to it inside an
	 * array so that it may be freed later. */
	person_t* persons[] = {
		person_new_heap("Alexander", 20),
		person_new_heap("Donald Knuth", 25),
		person_new_heap("Johnny Bravo", 16),
		person_new_heap("Knugen", 37),
		person_new_heap("N.C. Overguard", 37),
	};

	int n = sizeof(persons) / sizeof(persons[0]);

	/* Add the heap-allocated objects to the BST. Because BST_COPIED was
	 * used when creating the tree, these object will be copied to the
	 * tree. */
	for (int i = 0; i < n; ++i) {
		bst_add(bst, persons[i]);
	}

	bst_print(bst, person_print);

	bst		= bst_balanced(tmp);
	bst_free	(tmp);

	bst_print	(bst, person_print);

	/* Heap-allocated memory has to be explicitly freed by the caller! */
	for (int i = 0; i < n; ++i) {
		person_free_heap(persons[i]);
	}

	bst_print	(bst, person_print);

	bst_contains	(bst, persons[0]);
	bst_delete	(bst, persons[0]);
	bst_contains	(bst, persons[0]);

	/* Example showing that the data in the BST can outlive the data passed
	 * to the add-function. */
	bst_print	(bst, person_print);
	bst_free	(bst);

	printf("\n\n");
}


/*==============================================================================
	INT
==============================================================================*/

int int_cmp(const void* a, const void* b)
{
	return *((int*)a) - *((int*)b);
}

void int_print(void* data)
{
	if (data == NULL) {
		printf("int_print: Invalid data.\n");
		return;
	}
	printf("%d", *((int*)data));
}


/*==============================================================================
	PERSON
==============================================================================*/

person_t person_new_stack(const char* name, const int age)
{
	person_t person;
	strcpy(person.name, name);
	person.age = age;
	return person;
}

person_t* person_new_heap(const char* name, int age)
{
	person_t* p = malloc(sizeof(*p));
	strcpy(p->name, name);
	p->age = age;
	return p;
}

void person_free_heap(void* data)
{
	person_t* p = (person_t*) data;
	printf("  Freeing  ");
	person_print(p);
	printf("\n");
	free(p);
}

void person_print(void* person)
{
	person_t* p = (person_t*) person;

	printf("{ name = \"%s\", age = %d }", p->name, p->age);
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

