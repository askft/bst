#include "bst.h"
#include <stdio.h>
#include <string.h>

typedef struct {
	char	name[128];
	int	age;
} person_t ;


void test_person(void);
void test_int	(void);

person_t 	person_create	(const char* name, int age);
void		person_free	(void* data);
int		person_cmp	(const void* a, const void* b);
void		person_print	(void* data);

void		int_free	(void* data);
int		int_cmp		(const void* a, const void* b);
void		int_print	(void* data);

int main(void)
{
	test_person	();
	test_int	();
}


void test_person()
{
	printf( "----------------------------------------\n"
		" test_person\n"
		"----------------------------------------\n\n" );

	bst_t* bst	= bst_new(sizeof(person_t), person_cmp,
				  person_free, person_print);
	bst_t* tmp	= bst;

	person_t persons[] = {
		person_create("Alexander", 20),
		person_create("Donald Knuth", 25),
		person_create("Johnny Bravo", 16),
		person_create("Knugen", 37),
		person_create("N.C. Overguard", 37),
	};

	int n = sizeof(persons) / sizeof(persons[0]);

	for (int i = 0; i < n; ++i) {
		bst_add(bst, &persons[i]);
	}

	bst_print(bst, person_print);

	bst = bst_balanced(tmp);

	bst_free	(tmp);
	bst_print	(bst, person_print);
	bst_free	(bst);

	printf("\n\n");
}

void test_int()
{
	printf( "----------------------------------------\n"
		" test_int\n"
		"----------------------------------------\n\n" );
	bst_t*	bst;
	int	arr[] = { 5, 2, 3, 9, 6, 7, 1, 8, 4, 10 };

	bst = bst_new(sizeof(int), int_cmp, int_free, int_print);

	for (int i = 0; i < 10; ++i) {
		arr[i] = i + 1;
		bst_add(bst, &arr[i]);
	}

	bst_print	(bst, int_print);
	printf	("\nBalancing the tree...\n");
	bst_t* tmp = bst;
	bst = bst_balanced(tmp);
	bst_free(tmp);
	printf	("Balanced the tree!\n\n");
	bst_print	(bst, int_print);

	bst_free	(bst);

	printf("\n\n");
}


/*==============================================================================
	PERSON
==============================================================================*/

person_t person_create(const char* name, const int age)
{
	person_t person;
	strcpy(person.name, name);
	person.age = age;
	return person;
}

void person_print(void* person)
{
	person_t* p = (person_t*) person;

	printf("{ name = \"%s\", age = %d }", p->name, p->age);
}

/* TODO(Alexander): Make it more clear how stuff should be freed. In this
 * example, the data sent to the BST is stack-allocated and thus need not
 * be freed.
 */
void person_free(void* data)
{
//	person_t* p = (person_t*) data;
//	printf("  Freeing  ");
//	person_print(p);
//	printf("\n");
//	free(p);
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

void int_free(void* data)
{
}





#if 0
person_t* person_new(const char* name, int age)
{
	person_t* p = malloc(sizeof(*p));
	strcpy(p->name, name);
	p->age = age;
	return p;
}

	bst_add		(bst, person_new("Alexander", 20));
	bst_add		(bst, person_new("Donald Knuth", 25));
	bst_add		(bst, person_new("Johnny Bravo", 16));
	bst_add		(bst, person_new("Knugen", 37));
	bst_add		(bst, person_new("N.C. Overguard", 37));

	bst_print	(bst, person_print);

	printf("\nBalancing the tree...\n");

	bst_t* tmp;
	tmp		= bst;
	bst		= bst_balanced(tmp);
	bst_free	(tmp);

	printf("Balanced the tree!\n\n");

	bst_print	(bst, person_print);

	bst_free	(bst);

#endif
