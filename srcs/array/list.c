/*/
 * MetaReal version 1.0.0
/*/

#include <int.h>
#include <array/list.h>
#include <interpreter/operation.h>
#include <stdlib.h>

list_p list_set(value_p* elements, unsigned long long size)
{
    list_p array = malloc(sizeof(list_t));

    array->elements = elements;
    array->size = size;

    return array;
}

void list_free(list_p array)
{
    while (array->size)
        value_free(array->elements[--array->size]);

    free(array->elements);
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
    value_label(*array->elements, "");

    unsigned long long i;
    for (i = 1; i < array->size; i++)
    {
        fprintf(stream, ", ");
        value_label(array->elements[i], "");
    }

    fprintf(stream, "]%s", end);
}

list_p list_append(const list_p array, value_p value)
{
    list_p res = malloc(sizeof(list_t));

    if (!array->size)
    {
        res->elements = malloc(sizeof(value_p));
        res->size = 1;

        *res->elements = value;

        return res;
    }

    res->size = array->size + 1;
    res->elements = malloc(res->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < array->size; i++)
    {
        res->elements[i] = array->elements[i];
        value_copy(array->elements[i]);
    }

    res->elements[i] = value;

    return res;
}

void list_append_self(list_p array, value_p value)
{
    if (!array->size)
    {
        array->elements = malloc(sizeof(value_p));
        array->size = 1;

        *array->elements = value;
        return;
    }

    array->size++;
    array->elements = realloc(array->elements, array->size * sizeof(value_p));

    array->elements[array->size - 1] = value;
}

list_p list_concat(const list_p array1, const list_p array2)
{
    list_p res = malloc(sizeof(list_t));

    if (!array1->size)
    {
        if (!array2->size)
        {
            res->size = 0;
            return res;
        }

        unsigned long long i;
        for (i = 0; i < array2->size; i++)
        {
            res->elements[i] = array2->elements[i];
            value_copy(array2->elements[i]);
        }

        res->size = array2->size;

        return res;
    }

    if (!array2->size)
    {
        unsigned long long i;
        for (i = 0; i < array1->size; i++)
        {
            res->elements[i] = array1->elements[i];
            value_copy(array1->elements[i]);
        }

        res->size = array1->size;

        return res;
    }

    res->size = array1->size + array2->size;
    res->elements = malloc(res->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
    {
        res->elements[i] = array1->elements[i];
        value_copy(array1->elements[i]);
    }

    unsigned long long j;
    for (j = 0; j < array2->size; i++, j++)
    {
        res->elements[i] = array2->elements[j];
        value_copy(array2->elements[j]);
    }

    return res;
}

char list_concat_self(list_p array1, list_p array2)
{
    if (!array2->size)
        return 1;

    if (!array1->size)
    {
        array1->elements = array2->elements;
        array1->size = array2->size;

        return 1;
    }

    unsigned long long size = array1->size + array2->size;
    array1->elements = realloc(array1->elements, size * sizeof(value_p));

    unsigned long long i, j;
    for (i = array1->size, j = 0; j < array2->size; i++, j++)
    {
        array1->elements[i] = array2->elements[j];
        value_copy(array2->elements[j]);
    }

    array1->size = size;
    return 0;
}

list_p list_concat_tuple(const list_p array1, const tuple_p array2)
{
    list_p res = malloc(sizeof(list_t));

    if (!array1->size)
    {
        if (!array2->size)
        {
            res->size = 0;
            return res;
        }

        unsigned long long i;
        for (i = 0; i < array2->size; i++)
        {
            res->elements[i] = array2->elements[i];
            value_copy(array2->elements[i]);
        }

        res->size = array2->size;

        return res;
    }

    if (!array2->size)
    {
        unsigned long long i;
        for (i = 0; i < array1->size; i++)
        {
            res->elements[i] = array1->elements[i];
            value_copy(array1->elements[i]);
        }

        res->size = array1->size;

        return res;
    }

    res->size = array1->size + array2->size;
    res->elements = malloc(res->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
    {
        res->elements[i] = array1->elements[i];
        value_copy(array1->elements[i]);
    }

    unsigned long long j;
    for (j = 0; j < array2->size; i++, j++)
    {
        res->elements[i] = array2->elements[j];
        value_copy(array2->elements[j]);
    }

    return res;
}

char list_concat_tuple_self(list_p array1, tuple_p array2)
{
    list_p res = malloc(sizeof(list_t));

    if (!array2->size)
        return 1;

    if (!array2->size)
    {
        array1->elements = array2->elements;
        array1->size = array2->size;

        return 1;
    }

    unsigned long long size = array1->size + array2->size;
    array1->elements = realloc(array1->elements, size * sizeof(value_p));

    unsigned long long i, j;
    for (i = array1->size, j = 0; j < array2->size; i++, j++)
    {
        array1->elements[i] = array2->elements[j];
        value_copy(array2->elements[j]);
    }

    array1->size = size;
    return 0;
}

list_p list_remove(const list_p array, unsigned long long pos)
{
    list_p res = malloc(sizeof(list_t));

    if (array->size == 1)
    {
        res->size = 0;
        return res;
    }

    res->size = array->size - 1;
    res->elements = malloc(res->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < pos; i++)
    {
        res->elements[i] = array->elements[i];
        value_copy(array->elements[i]);
    }
    for (; i < array->size - 1; i++)
    {
        res->elements[i] = array->elements[i + 1];
        value_copy(res->elements[i]);
    }

    return res;
}

void list_remove_self(list_p array, unsigned long long pos)
{
    if (array->size == 1)
    {
        value_free(*array->elements);
        free(array->elements);

        array->size = 0;
        return;
    }

    value_free(array->elements[pos]);

    array->size--;

    unsigned long long i;
    for (i = pos; i < array->size; i++)
        array->elements[i] = array->elements[i + 1];

    array->elements = realloc(array->elements, array->size * sizeof(value_p));
}

list_p list_repeat(const list_p array, unsigned long long count)
{
    list_p res = malloc(sizeof(list_t));

    if (!count || !array->size)
    {
        res->size = 0;
        return res;
    }

    res->size = array->size * count;
    res->elements = malloc(res->size * sizeof(value_p));

    unsigned long long i, j;
    for (i = 0; i < res->size;)
        for (j = 0; j < array->size; i++, j++)
        {
            res->elements[i] = array->elements[j];
            value_copy(array->elements[j]);
        }

    return res;
}

void list_repeat_self(list_p array, unsigned long long count)
{
    if (!array->size || count == 1)
        return;

    if (!count)
    {
        while (array->size)
            value_free(array->elements[--array->size]);
        free(array->elements);

        return;
    }

    unsigned long long size = array->size * count;
    array->elements = realloc(array->elements, size * sizeof(value_p));

    unsigned long long i, j;
    for (i = array->size; i < size;)
        for (j = 0; j < array->size; i++, j++)
        {
            array->elements[i] = array->elements[j];
            value_copy(array->elements[j]);
        }

    array->size = size;
}

char list_equal(const list_p array1, const list_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(array1->elements[i], array2->elements[i]))
            return 0;

    return 1;
}

char list_equal_tuple(const list_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(array1->elements[i], array2->elements[i]))
            return 0;

    return 1;
}

char list_nequal(const list_p array1, const list_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(array1->elements[i], array2->elements[i]))
            return 1;

    return 0;
}

char list_nequal_tuple(const list_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_equal_compare(array1->elements[i], array2->elements[i]))
            return 1;

    return 0;
}

char list_contains(const list_p array, const value_p value)
{
    if (!array->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        if (operate_equal_compare(array->elements[i], value))
            return 1;

    return 0;
}

char list_are_type(const list_p array, unsigned char type)
{
    if (!array->size)
        return 1;

    if (type == NONE_V)
    {
        unsigned long long i;
        for (i = 0; i < array->size; i++)
            if (array->elements[i])
                return 0;

        return 1;
    }

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        if (!array->elements[i] || array->elements[i]->type != type)
            return 0;

    return 1;
}

list_p list_reverse(const list_p array)
{
    list_p res = malloc(sizeof(list_t));

    res->size = array->size;

    if (!array->size)
        return res;
    
    if (array->size == 1)
    {
        res->elements = malloc(sizeof(value_p));
        *res->elements = *array->elements;
        value_copy(*array->elements);

        return res;
    }

    res->elements = malloc(array->size * sizeof(value_p));

    unsigned long long i, j;
    for (i = 0, j = array->size - 1; i < array->size; i++, j--)
    {
        res->elements[i] = array->elements[j];
        value_copy(array->elements[j]);
    }

    return res;
}

void list_reverse_self(list_p array)
{
    if (array->size <= 1)
        return;

    value_p* start = array->elements;
    value_p* end = array->elements + array->size - 1;

    value_p tmp;
    while (start < end)
    {
        tmp = *start;

        *start++ = *end;
        *end-- = tmp;
    }
}
