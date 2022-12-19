/*/
 * MetaReal version 1.0.0
/*/

#include <array/list.h>
#include <interpreter/operation.h>
#include <stdlib.h>

list_p list_set(value_p elements, unsigned long long size)
{
    list_p array = malloc(sizeof(list_t));

    array->elements = elements;
    array->size = size;

    return array;
}

list_p list_copy(const list_p array)
{
    list_p copy = malloc(sizeof(list_t));

    copy->elements = malloc(array->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        copy->elements[i] = value_copy(&array->elements[i]);

    copy->size = array->size;

    return copy;
}

void list_free(list_p array)
{
    while (array->size)
        value_free(&array->elements[--array->size]);
    free(array);
}

void list_free_exception(list_p array, unsigned long long exception)
{
    while (array->size)
    {
        if (--array->size == exception)
            continue;
        value_free(&array->elements[array->size]);
    }

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

void list_append(list_p array1, value_p value)
{
    if (!array1->size)
    {
        array1->size = 1;
        array1->elements = malloc(sizeof(value_t));

        *array1->elements = *value;
        return;
    }

    array1->size++;
    array1->elements = realloc(array1->elements, array1->size * sizeof(value_t));

    array1->elements[array1->size - 1] = *value;
}

void list_concat(list_p array1, const list_p array2)
{
    if (!array1->size)
    {
        array1->elements = array2->elements;
        array1->size = array2->size;
        return;
    }

    unsigned long long size = array1->size + array2->size;
    array1->elements = realloc(array1->elements, size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array2->size; i++)
        array1->elements[array1->size + i] = array2->elements[i];

    array1->size = size;
}

void list_concat_tuple(list_p array1, const tuple_p array2)
{
    if (!array1->size)
    {
        array1->elements = array2->elements;
        array1->size = array2->size;
        return;
    }

    unsigned long long size = array1->size + array2->size;
    array1->elements = realloc(array1->elements, size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < array2->size; i++)
        array1->elements[array1->size + i] = array2->elements[i];

    array1->size = size;
}

void list_remove(list_p array, unsigned long long pos)
{
    if (array->size == 1)
    {
        free(array->elements);
        array->size = 0;
        return;
    }

    array->size--;

    unsigned long long i;
    for (i = pos; i < array->size; i++)
        array->elements[i] = array->elements[i + 1];

    array->elements = realloc(array->elements, array->size * sizeof(value_t));
}

void list_repeat(list_p array, unsigned long long count)
{
    if (!count)
    {
        if (array->size)
        {
            free(array->elements);
            array->size = 0;
        }
        return;
    }

    if (count == 1)
        return;

    if (!array->size)
        return;

    unsigned long long size = array->size * count;
    array->elements = realloc(array->elements, size * sizeof(value_t));

    if (array->size == 1)
    {
        unsigned long long i;
        for (i = 1; i < size; i++)
            array->elements[i] = value_copy(array->elements);

        array->size = size;
        return;
    }

    unsigned long long i, j;
    for (i = array->size; i < size;)
        for (j = 0; j < array->size; i++, j++)
            array->elements[i] = value_copy(&array->elements[j]);

    array->size = size;
}

char list_equal(const list_p array1, const list_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_compare(&array1->elements[i], &array2->elements[i]))
            return 0;

    return 1;
}

char list_equal_tuple(const list_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_compare(&array1->elements[i], &array2->elements[i]))
            return 0;

    return 1;
}

char list_nequal(const list_p array1, const list_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_compare(&array1->elements[i], &array2->elements[i]))
            return 1;

    return 0;
}

char list_nequal_tuple(const list_p array1, const tuple_p array2)
{
    if (array1->size != array2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < array1->size; i++)
        if (!operate_compare(&array1->elements[i], &array2->elements[i]))
            return 1;

    return 0;
}

char list_contains(const list_p array, const value_p value)
{
    if (!array->size)
        return 0;

    unsigned long long i;
    for (i = 0; i < array->size; i++)
        if (operate_compare(&array->elements[i], value))
            return 1;

    return 0;
}

void list_reverse(list_p array)
{
    if (array->size <= 1)
        return;

    value_p start = array->elements;
    value_p end = array->elements + array->size - 1;

    while (start < end)
    {
        value_t temp = *start;

        *start++ = *end;
        *end-- = temp;
    }
}

unsigned long long list_size(const list_p array)
{
    return array->size;
}
