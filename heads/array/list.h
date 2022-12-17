/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_LIST__
#define __M_LIST__

#include <array/tuple.h>

struct __list__
{
    value_p elements;
    unsigned long long size;
};
typedef struct __list__ list_t;
typedef struct __list__* list_p;

list_p list_set(value_p elements, unsigned long long size);

list_p list_copy(const list_p array);

void list_free(list_p array);
void list_free_exception(list_p array, unsigned long long exception);

void list_print(FILE* stream, const list_p array, const char* end);

/* */

void list_append(list_p array1, value_p value);

void list_concat(list_p array1, list_p array2);
void list_concat_tuple(list_p array1, tuple_p array2);

void list_remove(list_p array, unsigned long long pos);

void list_repeat(list_p array, unsigned long long count);

char list_equal(const list_p array1, const list_p array2);
char list_equal_tuple(const list_p array1, const tuple_p array2);

char list_nequal(const list_p array1, const list_p array2);
char list_nequal_tuple(const list_p array1, const tuple_p array2);

char list_contains(const list_p array, const value_p value);

/* */

void list_reverse(list_p array);

/* */

unsigned long long list_size(const list_p array);

#endif /* __M_LIST__ */
