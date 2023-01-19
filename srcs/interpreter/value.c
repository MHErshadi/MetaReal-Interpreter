/*/
 * MetaReal version 1.0.0
/*/

#include <complex.h>
#include <str.h>
#include <array/list.h>
#include <structure/function.h>
#include <setting.h>
#include <stdlib.h>

value_p value_set1(unsigned char type, void* ptr)
{
    value_p value = malloc(sizeof(value_t));

    value->type = type;
    value->value.ptr = ptr;
    value->ref = 0;

    return value;
}

value_p value_set2(unsigned char type, char chr)
{
    value_p value = malloc(sizeof(value_t));

    value->type = type;
    value->value.chr = chr;
    value->ref = 0;

    return value;
}

void value_free(value_p value)
{
    if (!value)
        return;

    if (value->ref)
    {
        value->ref--;
        return;
    }

    switch (value->type)
    {
    case OBJECT_V:
    case BOOL_V:
    case CHAR_V:
    case TYPE_V:
        break;
    case INT_V:
        int_free(value->value.ptr);
        break;
    case FLOAT_V:
        float_free(value->value.ptr);
        break;
    case COMPLEX_V:
        complex_free(value->value.ptr);
        break;
    case STR_V:
        str_free(value->value.ptr);
        break;
    case LIST_V:
        list_free(value->value.ptr);
        break;
    case TUPLE_V:
        tuple_free(value->value.ptr);
        break;
    case FUNC_V:
        func_free(value->value.ptr);
        break;
    case STRUCT_V:
        context_free(value->value.ptr);
        break;
    }

    free(value);
}

void value_label(value_p value, const char* end)
{
    if (!value)
    {
        fprintf(setting.output, "none%s", end);
        return;
    }

    switch (value->type)
    {
    case OBJECT_V:
        fprintf(setting.output, "<object at 0x%p>%s", value->value.ptr, end);
        return;
    case INT_V:
        int_print(setting.output, value->value.ptr, end);
        return;
    case FLOAT_V:
        float_print(setting.output, value->value.ptr, end);
        return;
    case COMPLEX_V:
        complex_print(setting.output, value->value.ptr, end);
        return;
    case BOOL_V:
        fprintf(setting.output, value->value.chr ? "true%s" : "false%s", end);
        return;
    case CHAR_V:
        switch (value->value.chr)
        {
        case '\0':
            fprintf(setting.output, "'\\0'%s", end);
            return;
        case '\a':
            fprintf(setting.output, "'\\a'%s", end);
            return;
        case '\b':
            fprintf(setting.output, "'\\b'%s", end);
            return;
        case '\f':
            fprintf(setting.output, "'\\f'%s", end);
            return;
        case '\n':
            fprintf(setting.output, "'\\n'%s", end);
            return;
        case '\r':
            fprintf(setting.output, "'\\r'%s", end);
            return;
        case '\t':
            fprintf(setting.output, "'\\t'%s", end);
            return;
        case '\v':
            fprintf(setting.output, "'\\v'%s", end);
            return;
        default:
            fprintf(setting.output, "'%c'%s", value->value.chr, end);
            return;
        }
    case STR_V:
        str_label(setting.output, value->value.ptr, end);
        return;
    case LIST_V:
        list_print(setting.output, value->value.ptr, end);
        return;
    case TUPLE_V:
        tuple_print(setting.output, value->value.ptr, end);
        return;
    case TYPE_V:
        fprintf(setting.output, "<type %s>%s", value_labels[value->value.chr], end);
        return;
    case FUNC_V:
        context_print(setting.output, "function", &func_context(value->value.ptr), end);
        return;
    case STRUCT_V:
        context_print(setting.output, "struct", value->value.ptr, end);
        return;
    }
}

char value_is_true(value_p value)
{
    if (!value)
        return 0;

    switch (value->type)
    {
    case OBJECT_V:
        return 1;
    case INT_V:
        return int_sign(value->value.ptr) != 0;
    case FLOAT_V:
        return float_sign(value->value.ptr) != 0;
    case COMPLEX_V:
        return !complex_is_zero(value->value.ptr);
    case BOOL_V:
        return value->value.chr;
    case CHAR_V:
        return value->value.chr != 0;
    case STR_V:
        return str_size(value->value.ptr) != 0;
    case LIST_V:
        return list_size(value->value.ptr) != 0;
    case TUPLE_V:
        return tuple_size(value->value.ptr) != 0;
    case TYPE_V:
        return value->value.chr != NONE_V;
    case FUNC_V:
        return func_name(value->value.ptr) != NULL;
    case STRUCT_V:
        return context_name(value->value.ptr) != NULL;
    }

    return 0;
}
