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

void tuple_print(FILE* stream, tuple_p array, const char* end);

/* */

char tuple_equal(tuple_p array1, tuple_p array2);
char tuple_nequal(tuple_p array1, tuple_p array2);

#endif /* __M_TUPLE__ */
