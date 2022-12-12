/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <complex.h>
#include <str.h>
#include <setting.h>
#include <stdlib.h>

runtime_t illegal_operation(unsigned char type1, unsigned char type2, const char* operator, pos_p poss, pos_p pose, context_p context);

ires_t operate_add(value_p left, value_p right)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            left->pose = right->pose;

            int_add(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            right->poss = left->poss;

            float_add_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            right->poss = left->poss;

            complex_add_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case STR_V:
            right->poss = left->poss;

            char* int_str = int_get_str(left->value.ptr);

            str_str_concat(int_str, right->value.ptr);
            free(int_str);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            left->pose = right->pose;

            float_add_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->pose = right->pose;

            float_add(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            right->poss = left->poss;

            complex_add_float(right->value.ptr, left->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case STR_V:
            right->poss = left->poss;

            char* float_str = float_get_str(left->value.ptr, setting.float_prec_show);

            str_str_concat(float_str, right->value.ptr);
            free(float_str);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            left->pose = right->pose;

            complex_add_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->pose = right->pose;

            complex_add_float(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            left->pose = right->pose;

            complex_add(left->value.ptr, right->value.ptr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case STR_V:
            right->poss = left->poss;

            char* complex_str = complex_get_str(left->value.ptr, setting.complex_prec_show);

            str_str_concat(complex_str, right->value.ptr);
            free(complex_str);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        if (right->type == STR_V)
        {
            right->poss = left->poss;

            str_str_concat(left->value.chr ? "true" : "false", right->value.ptr);

            return ires_success(right);
        }

        break;
    case CHAR_V:
        if (right->type == STR_V)
        {
            right->poss = left->poss;

            str_char_concat(left->value.chr, right->value.ptr);

            return ires_success(right);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            left->pose = right->pose;

            char* int_str = int_get_str(right->value.ptr);

            str_concat_str(left->value.ptr, int_str);
            free(int_str);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->pose = right->pose;

            char* float_str = float_get_str(right->value.ptr, setting.float_prec_show);

            str_concat_str(left->value.ptr, float_str);
            free(float_str);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            left->pose = right->pose;

            char* complex_str = complex_get_str(right->value.ptr, setting.complex_prec_show);

            str_concat_str(left->value.ptr, complex_str);
            free(complex_str);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            left->pose = right->pose;

            str_concat_str(left->value.ptr, right->value.chr ? "true" : "false");

            return ires_success(left);
        case CHAR_V:
            left->pose = right->pose;

            str_concat_char(left->value.ptr, right->value.chr);

            return ires_success(left);
        case STR_V:
            left->pose = right->pose;

            str_concat(left->value.ptr, right->value.ptr);
            str_free(right->value.ptr);

            return ires_success(left);
        }

        break;
    }

    runtime_t error = illegal_operation(left->type, right->type, "+", &left->poss, &right->pose, left->context);
    return ires_fail(&error);
}

ires_t operate_subtract(value_p left, value_p right)
{

}

ires_t operate_multiply(value_p left, value_p right)
{

}

ires_t operate_divide(value_p left, value_p right)
{

}

ires_t operate_modulo(value_p left, value_p right)
{

}

ires_t operate_quotient(value_p left, value_p right)
{

}

ires_t operate_power(value_p left, value_p right)
{

}

ires_t operate_b_and(value_p left, value_p right)
{

}

ires_t operate_b_or(value_p left, value_p right)
{

}

ires_t operate_b_xor(value_p left, value_p right)
{

}

ires_t operate_b_lshift(value_p left, value_p right)
{

}

ires_t operate_b_rshift(value_p left, value_p right)
{

}

ires_t operate_equal(value_p left, value_p right)
{

}

ires_t operate_nequal(value_p left, value_p right)
{

}

ires_t operate_less(value_p left, value_p right)
{

}

ires_t operate_greater(value_p left, value_p right)
{

}

ires_t operate_less_eq(value_p left, value_p right)
{

}

ires_t operate_greater_eq(value_p left, value_p right)
{

}

ires_t operate_and(value_p left, value_p right)
{

}

ires_t operate_or(value_p left, value_p right)
{

}

ires_t operate_xor(value_p left, value_p right)
{

}

ires_t operate_negate(value_p operand)
{

}

ires_t operate_b_not(value_p operand)
{

}

ires_t operate_not(value_p operand)
{

}

ires_t operate_increment(value_p operand)
{

}

ires_t operate_decrement(value_p operand)
{

}

runtime_t illegal_operation(unsigned char type1, unsigned char type2, const char* operator, pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(39 + value_label_lens[type1] + value_label_lens[type2]);
    sprintf(detail, "Illegal operation (%s) between <%s> and <%s>", operator, value_labels[type1], value_labels[type2]);

    return runtime_set(ILLEGAL_OPERATION_E, detail, poss, pose, context);
}
