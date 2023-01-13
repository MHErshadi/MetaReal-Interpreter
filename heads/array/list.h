/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_LIST__
#define __M_LIST__

#include "tuple.h"

struct __list__
{
    value_p* elements;
    unsigned long long size;
};
typedef struct __list__ list_t;
typedef struct __list__* list_p;

#define list_size(array) (((list_p)(array))->size)

/* */

list_p list_set(value_p* elements, unsigned long long size);

void list_free(list_p array);

void list_print(FILE* stream, const list_p array, const char* end);

/* */

list_p list_append(const list_p array, value_p value);
void list_append_self(list_p array, value_p value);

list_p list_concat(const list_p array1, const list_p array2);
char list_concat_self(list_p array1, list_p array2);

list_p list_concat_tuple(const list_p array1, const tuple_p array2);
char list_concat_tuple_self(list_p array1, tuple_p array2);

list_p list_remove(const list_p array, unsigned long long pos);
void list_remove_self(list_p array, unsigned long long pos);

list_p list_repeat(const list_p array, unsigned long long count);
void list_repeat_self(list_p array, unsigned long long count);

char list_equal(const list_p array1, const list_p array2);
char list_equal_tuple(const list_p array1, const tuple_p array2);

char list_nequal(const list_p array1, const list_p array2);
char list_nequal_tuple(const list_p array1, const tuple_p array2);

char list_contains(const list_p array, const value_p value);

char list_are_type(const list_p array, unsigned char type);

/* */

list_p list_reverse(const list_p array);
void list_reverse_self(list_p array);

#endif /* __M_LIST__ */
