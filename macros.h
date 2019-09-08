#ifndef MACROS_H
#define MACROS_H


/*
 * If DEBUG != 0, activate debug mode (i.e. print more stuff)
 */
#define DEBUG	0


/* Error and debug messages */
#define OUT_OF_MEMORY	"Out of memory.\n"

#define MALLOC_FAIL	"`malloc` failed.\n"


/*
 * Function-like macros
 * 	ERROR:		Print location of error and exit with failure status.
 * 	DEBUG_VAR:	Print variable name and value.
 */

#define ERROR(STATEMENT, ...)						    \
do {									    \
	fprintf(stderr, "Error in file \"%s\", line %d, function \"%s\":\n",\
			__FILE__, __LINE__, __func__);			    \
	fprintf(stderr, __VA_ARGS__);					    \
	STATEMENT;							    \
} while (0);

#define ERROR_EXIT(...)							    \
do {									    \
	fprintf(stderr, "Error in file \"%s\", line %d, function \"%s\":\n",\
			__FILE__, __LINE__, __func__);			    \
	fprintf(stderr, __VA_ARGS__);					    \
	exit(EXIT_FAILURE);						    \
} while (0);

#if DEBUG
#define DEBUG_VAR(pre, x, post, var_format)			\
do {								\
	fprintf(stderr, pre "%s\t= "var_format post, #x, x);	\
} while (0);
#else
#define DEBUG_VAR(pre, x, post, var_format)
#endif

#if DEBUG
#define DEBUG_PRINT(...)					\
do {								\
	fprintf(stderr, "%s line %d: ", __FILE__, __LINE__);	\
	fprintf(stderr, __VA_ARGS__);				\
} while (0);
#else
#define DEBUG_PRINT(...)
#endif


#endif /* MACROS_H */

