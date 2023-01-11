/*/
 * MetaReal version 1.0.0
/*/

#include <int.h>
#include <array/list.h>
#include <interpreter/operation.h>
#include <stdlib.h>

list_p list_set(value_p elements, unsigned long long size)
{
    list_p array = malloc(sizeof(list_t));

    array->elements = elements;
    array->size = size;
    array->ref = 0;

    return array;
}

void list_free(list_p array)
{
    if (array->ref)
    {
        array->ref--;
        return;
    }

    while (array->size)
        value_free(array->elements + --array->size);
    free(array);
}

void list_print(FILE* stream, const list_p array, const char* end)
{
    if (!array->size)
    {
        fprintf(stream, "[]%s", end);
        return;
    }

    fputc('[', stream);
    value_label(array->elements, "");

    unsigned long long i;
    for (i = 1; i < array->size; i++)
    {
        fprintf(stream, ", ");
        value_label(&array->elements[i], "");
    }

    fprintf(stream, "]%s", end);
}

list_p list_append(const list_p array, value_p value)
{
    list_p res = malloc(sizeof(list_t));

    res->ref = 0;

    if (!array->size)
    {
        res->elements = malloc(sizeof(value_t));
        res->size = 1;

        *res->elements = *value;

        return res;
    }

    res->size = array->size + 1;
    res->elements = malloc(res->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        res->elements[i] = value_copy(array->elements + i);
    res->elements[i] = *value;

    return res;
}

list_p list_concat(const list_p array1, const list_p array2)
{
    list_p res = malloc(sizeof(list_t));

    res->ref = 0;

    if (!array1->size)
    {
        if (!array2->size)
        {
            res->size = 0;
            return res;
        }

        unsigned long long i;
        for (i = 0; i < array2->size; i++)
            res->elements[i] = value_copy(array2->elements + i);

        res->size = array2->size;

        return res;
    }

    if (!array2->size)
    {
        unsigned long long i;
        for (i = 0; i < array1->size; i++)
            res->elements[i] = value_copy(array1->elements + i);

        res->size = array1->size;

        return res;
    }

    res->size = array1->size + array2->size;
    res->elements = malloc(res->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        res->elements[i] = value_copy(array1->elements + i);

    unsigned long long j;
    for (j = 0; j < array2->size; i++, j++)
        res->elements[i] = value_copy(array2->elements + j);

    return res;
}

list_p list_concat_tuple(const list_p array1, const tuple_p array2)
{
    list_p res = malloc(sizeof(list_t));

    res->ref = 0;

    if (!array1->size)
    {
        if (!array2->size)
        {
            res->size = 0;
            return res;
        }

        unsigned long long i;
        for (i = 0; i < array2->size; i++)
            res->elements[i] = value_copy(array2->elements + i);

        res->size = array2->size;

        return res;
    }

    if (!array2->size)
    {
        unsigned long long i;
        for (i = 0; i < array1->size; i++)
            res->elements[i] = value_copy(array1->elements + i);

        res->size = array1->size;

        return res;
    }

    res->size = array1->size + array2->size;
    res->elements = malloc(res->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        res->elements[i] = value_copy(array1->elements + i);

    unsigned long long j;
    for (j = 0; j < array2->size; i++, j++)
        res->elements[i] = value_copy(array2->elements + j);

    return res;
}

list_p list_remove(const list_p array, unsigned long long pos)
{
    list_p res = malloc(sizeof(list_t));

    res->ref = 0;

    if (array->size == 1)
    {
        res->size = 0;
        return res;
    }

    res->size = array->size - 1;
    res->elements = malloc(res->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < pos; i++)
        res->elements[i] = value_copy(array->elements + i);
    for (; i < array->size - 1; i++)
        res->elements[i] = value_copy(array->elements + i + 1);

    return res;
}

list_p list_repeat(const list_p array, unsigned long long count)
{
    list_p res = malloc(sizeof(list_t));

    res->ref = 0;

    if (!count || !array->size)
    {
        res->size = 0;
        return res;
    }

    res->size = array->size * count;
    res->elements = malloc(res->size * sizeof(value_t));

    unsigned long long i, j;
    for (i = 0; i < res->size;)
        for (j = 0; j < array->size; i++, j++)
            res->elements[i] = value_copy(array->elements + j);

    return res;
}

char list_equal(const list_p array1, const list_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(&array1->elements[i], &array2->elements[i]))
            return 0;

    return 1;
}

char list_equal_tuple(const list_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(&array1->elements[i], &array2->elements[i]))
            return 0;

    return 1;
}

char list_nequal(const list_p array1, const list_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(&array1->elements[i], &array2->elements[i]))
            return 1;

    return 0;
}

char list_nequal_tuple(const list_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(&array1->elements[i], &array2->elements[i]))
            return 1;

    return 0;
}

char list_contains(const list_p array, const value_p value)
{
    if (!array->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        if (operate_equal_compare(&array->elements[i], value))
            return 1;

    return 0;
}

list_p list_reverse(const list_p array)
{
    list_p res = malloc(sizeof(list_t));

    res->size = array->size;
    res->ref = 0;

    if (!array->size)
        return res;
    
    if (array->size == 1)
    {
        res->elements = malloc(sizeof(value_t));
        *res->elements = value_copy(array->elements);

        return res;
    }

    res->elements = malloc(array->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        res->elements[i] = value_copy(array->elements + array->size - i - 1);

    return res;
}
