/*/
 * MetaReal version 1.0.0
/*/

#include <array/tuple.h>
#include <interpreter/operation.h>
#include <stdlib.h>

tuple_p tuple_set(value_p* elements, unsigned long long size)
{
    tuple_p array = malloc(sizeof(tuple_t));

    array->elements = elements;
    array->size = size;

    return array;
}

void tuple_free(tuple_p array)
{
    while (array->size)
        value_free(array->elements[--array->size]);

    free(array->elements);
    free(array);
}

void tuple_print(FILE* stream, const tuple_p array, const char* end)
{
    if (!array->size)
    {
        fprintf(stream, "()%s", end);
        return;
    }

    fputc('(', stream);
    value_label(*array->elements, "");

    unsigned long long i;
    for (i = 1; i < array->size; i++)
    {
        fprintf(stream, ", ");
        value_label(array->elements[i], "");
    }

    fprintf(stream, ")%s", end);
}

char tuple_equal(const tuple_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        //if (!operate_equal_compare(array1->elements[i], array2->elements[i]))
            return 0;

    return 1;
}

char tuple_nequal(const tuple_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        //if (!operate_equal_compare(array1->elements[i], array2->elements[i]))
            return 1;

    return 0;
}

char tuple_contains(const tuple_p array, const value_p value)
{
    if (!array->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        //if (operate_equal_compare(array->elements[i], value))
            return 1;

    return 0;
}
