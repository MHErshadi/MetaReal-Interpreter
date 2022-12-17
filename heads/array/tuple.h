/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_TUPLE__
#define __M_TUPLE__

#include <interpreter/value.h>

struct __tuple__
{
    value_p elements;
    unsigned long long size;
};
typedef struct __tuple__ tuple_t;
typedef struct __tuple__* tuple_p;

tuple_p tuple_set(value_p elements, unsigned long long size);

void tuple_free(tuple_p array);
void tuple_free_exception(tuple_p array, unsigned long long exception);

void tuple_print(FILE* stream, const tuple_p array, const char* end);

/* */

char tuple_equal(const tuple_p array1, const tuple_p array2);
char tuple_nequal(const tuple_p array1, const tuple_p array2);

char tuple_contains(const tuple_p array, const value_p value);

/* */

unsigned long long tuple_size(const tuple_p array);

#endif /* __M_TUPLE__ */
