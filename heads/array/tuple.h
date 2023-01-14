/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_TUPLE__
#define __M_TUPLE__

#include <interpreter/value.h>
#include <stdio.h>

struct __tuple__
{
    value_p* elements;
    unsigned long long size;
};
typedef struct __tuple__ tuple_t;
typedef struct __tuple__* tuple_p;

#define tuple_elements(array) (((tuple_p)(array))->elements)
#define tuple_size(array) (((tuple_p)(array))->size)

/* */

tuple_p tuple_set(value_p* elements, unsigned long long size);
tuple_p tuple_copy(const tuple_p src);

void tuple_free(tuple_p array);

void tuple_print(FILE* stream, const tuple_p array, const char* end);

/* */

char tuple_equal(const tuple_p array1, const tuple_p array2);

char tuple_nequal(const tuple_p array1, const tuple_p array2);

char tuple_contains(const tuple_p array, const value_p value);

char tuple_are_type(const tuple_p array, unsigned char type);

#endif /* __M_TUPLE__ */
