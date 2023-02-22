/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/type_call.h>
#include <int.h>
#include <str.h>
#include <array/list.h>
#include <stdlib.h>

ires_t object_call();
ires_t int_call(value_p* args, unsigned long long size);
ires_t list_call(value_p* args, unsigned long long size);
ires_t tuple_call(value_p* args, unsigned long long size);

ires_t handle_type_call(unsigned char id, value_p* args, unsigned long long size)
{
    switch (id)
    {
    case NONE_V:
        return ires_success(NULL);
    case OBJECT_V:
        return object_call();
    case INT_V:
        return int_call(args, size);
    case LIST_V:
        return list_call(args, size);
    case TUPLE_V:
        return tuple_call(args, size);
    }
}

ires_t object_call()
{
    value_p obj = malloc(sizeof(value_t));

    obj->type = OBJECT_V;
    obj->value.ptr = obj;
    obj->ref = 0;

    return ires_success(obj);
}

ires_t int_call(value_p* args, unsigned long long size)
{
    return ires_success(value_set1(INT_V, int_set_str(
        str_str((*args)->value.ptr), str_size((*args)->value.ptr), 10)));
}

ires_t list_call(value_p* args, unsigned long long size)
{
    value_p* elements = malloc(size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        elements[i] = args[i];
        value_copy(args[i]);
    }

    return ires_success(value_set1(LIST_V, list_set(elements, size)));
}

ires_t tuple_call(value_p* args, unsigned long long size)
{
    value_p* elements = malloc(size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        elements[i] = args[i];
        value_copy(args[i]);
    }

    return ires_success(value_set1(TUPLE_V, tuple_set(elements, size)));
}
