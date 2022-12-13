/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/value.h>
#include <complex.h>
#include <str.h>
#include <setting.h>

value_t value_set1(unsigned char type, void* ptr, pos_p poss, pos_p pose, context_p context)
{
    value_t value;

    value.type = type;
    value.value.ptr = ptr;
    value.poss = *poss;
    value.pose = *pose;
    value.context = context;

    return value;
}

value_t value_set2(unsigned char type, char chr, pos_p poss, pos_p pose, context_p context)
{
    value_t value;

    value.type = type;
    value.value.chr = chr;
    value.poss = *poss;
    value.pose = *pose;
    value.context = context;

    return value;
}

value_t value_set3(unsigned char type, pos_p poss, pos_p pose, context_p context)
{
    value_t value;

    value.type = type;
    value.poss = *poss;
    value.pose = *pose;
    value.context = context;

    return value;
}

void value_free(value_p value)
{
    switch (value->type)
    {
    case NULL_V:
    case OBJECT_V:
    case NONE_V:
    case BOOL_V:
    case CHAR_V:
        return;
    case INT_V:
        int_free(value->value.ptr);
        return;
    case FLOAT_V:
        float_free(value->value.ptr);
        return;
    case COMPLEX_V:
        complex_free(value->value.ptr);
        return;
    case STR_V:
        str_free(value->value.ptr);
        return;
    }
}

void value_label(value_p value, const char* end)
{
    switch (value->type)
    {
    case NULL_V:
        return;
    case OBJECT_V:
        fprintf(setting.output, "<object at 0X%p>%s", value, end);
        return;
    case NONE_V:
        fprintf(setting.output, "none%s", end);
        return;
    case INT_V:
        int_print(setting.output, value->value.ptr, end);
        return;
    case FLOAT_V:
        float_print(setting.output, value->value.ptr, setting.float_prec_show, end);
        return;
    case COMPLEX_V:
        complex_print(setting.output, value->value.ptr, setting.complex_prec_show, end);
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
    }
}

char value_is_true(value_p value)
{
    switch (value->type)
    {
    case NULL_V:
    case NONE_V:
        return 0;
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
        return ((str_p)value->value.ptr)->size != 0;
    }
}
