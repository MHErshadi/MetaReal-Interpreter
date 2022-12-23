/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <complex.h>
#include <str.h>
#include <array/list.h>
#include <setting.h>
#include <stdlib.h>
#include <string.h>

runtime_t illegal_operation_error(unsigned char type1, unsigned char type2, const char* operator, pos_p poss, pos_p pose, context_p context);
runtime_t illegal_operation_unary_error(unsigned char type, const char* operator, pos_p poss, pos_p pose, context_p context);
runtime_t mem_overflow_error(pos_p poss, pos_p pose, context_p context);
runtime_t out_of_range_error(pos_p poss, pos_p pose, context_p context);
runtime_t division_by_zero_error(pos_p poss, pos_p pose, context_p context);
runtime_t modulo_by_zero_error(pos_p poss, pos_p pose, context_p context);

ires_t operate_add(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char* str;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_add(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            float_add_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_add_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            int_add_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        case STR_V:
            str = int_get_str(left->value.ptr);

            str_str_concat(str, right->value.ptr);
            free(str);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            float_add_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            float_add(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_add_float(right->value.ptr, left->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            float_add_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        case STR_V:
            str = float_get_str(left->value.ptr, setting.float_prec_show);

            str_str_concat(str, right->value.ptr);
            free(str);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            complex_add_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            complex_add_float(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_add(left->value.ptr, right->value.ptr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            complex_add_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        case STR_V:
            str = complex_get_str(left->value.ptr, setting.complex_prec_show);

            str_str_concat(str, right->value.ptr);
            free(str);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            int_add_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case FLOAT_V:
            float_add_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case COMPLEX_V:
            complex_add_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
            left->value.chr ^= right->value.chr;

            return ires_success(left);
        case CHAR_V:
            right->value.chr += left->value.chr;

            return ires_success(right);
        case STR_V:
            str_str_concat(left->value.chr ? "true" : "false", right->value.ptr);

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_add_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case FLOAT_V:
            float_add_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case COMPLEX_V:
            complex_add_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr += right->value.chr;

            return ires_success(left);
        case STR_V:
            str_char_concat(left->value.chr, right->value.ptr);

            return ires_success(right);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            str = int_get_str(right->value.ptr);

            str_concat_str(left->value.ptr, str);
            free(str);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            str = float_get_str(right->value.ptr, setting.float_prec_show);

            str_concat_str(left->value.ptr, str);
            free(str);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            str = complex_get_str(right->value.ptr, setting.complex_prec_show);

            str_concat_str(left->value.ptr, str);
            free(str);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            str_concat_str(left->value.ptr, right->value.chr ? "true" : "false");

            return ires_success(left);
        case CHAR_V:
            str_concat_char(left->value.ptr, right->value.chr);

            return ires_success(left);
        case STR_V:
            str_concat(left->value.ptr, right->value.ptr);
            str_free(right->value.ptr);

            return ires_success(left);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            list_concat(left->value.ptr, right->value.ptr);
            free(right->value.ptr);

            return ires_success(left);
        case TUPLE_V:
            list_concat_tuple(left->value.ptr, right->value.ptr);
            free(right->value.ptr);

            return ires_success(left);
        }

        list_append(left->value.ptr, right);

        return ires_success(left);
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "+", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_subtract(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_subtract(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            float_int_subtract(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_int_subtract(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            int_subtract_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            float_subtract_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            float_subtract(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_float_subtract(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            float_subtract_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            complex_subtract_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            complex_subtract_float(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_subtract(left->value.ptr, right->value.ptr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            complex_subtract_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            int_ul_subtract(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            float_ul_subtract(left->value.chr, right->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_ul_subtract(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
            left->value.chr ^= right->value.chr;

            return ires_success(left);
        case CHAR_V:
            right->value.chr = left->value.chr - right->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_ul_subtract(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            float_ul_subtract(left->value.chr, right->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_ul_subtract(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr -= right->value.chr;

            return ires_success(left);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            unsigned long long index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((str_p)left->value.ptr)->size - index;

            if (index >= ((str_p)left->value.ptr)->size)
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            str_remove(left->value.ptr, index);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= ((str_p)left->value.ptr)->size)
            {
                str_free(left->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            str_remove(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                list_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            unsigned long long index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((list_p)left->value.ptr)->size - index;

            if (index >= ((list_p)left->value.ptr)->size)
            {
                list_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            list_remove(left->value.ptr, index);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= ((list_p)left->value.ptr)->size)
            {
                list_free(left->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            list_remove(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "-", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_multiply(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_multiply(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            float_multiply_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_multiply_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            int_multiply_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            float_multiply_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            float_multiply(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_multiply_float(right->value.ptr, left->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            float_multiply_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            complex_multiply_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            complex_multiply_float(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_multiply(left->value.ptr, right->value.ptr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            complex_multiply_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            int_multiply_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case FLOAT_V:
            float_multiply_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case COMPLEX_V:
            complex_multiply_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
            left->value.chr &= right->value.chr;

            return ires_success(left);
        case CHAR_V:
            right->value.chr *= left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_multiply_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case FLOAT_V:
            float_multiply_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case COMPLEX_V:
            complex_multiply_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr *= right->value.chr;

            return ires_success(left);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            if (int_sign(right->value.ptr) < 0)
                str_reverse(left->value.ptr);

            unsigned long long count = int_get_ull(right->value.ptr);

            str_repeat(left->value.ptr, count);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            str_repeat(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                list_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            if (int_sign(right->value.ptr) < 0)
                list_reverse(left->value.ptr);

            unsigned long long count = int_get_ull(right->value.ptr);

            list_repeat(left->value.ptr, count);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            list_repeat(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "*", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_divide(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    float_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_int_divide_int(left->value.ptr, right->value.ptr, setting.float_prec_bit);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = FLOAT_V;
            left->value.ptr = res;

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                int_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_int_divide(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                int_free(left->value.ptr);
                complex_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_int_divide(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_int_divide_ul(left->value.ptr, right->value.chr, setting.complex_prec_bit);
            int_free(left->value.ptr);

            left->type = FLOAT_V;
            left->value.ptr = res;

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                float_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_divide_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_divide(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                float_free(left->value.ptr);
                complex_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_float_divide(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                float_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                float_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_divide_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                complex_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_divide_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                complex_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_divide_float(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                complex_free(left->value.ptr);
                complex_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_divide(left->value.ptr, right->value.ptr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                complex_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                complex_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_divide_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_ul_divide_int(left->value.chr, right->value.ptr, setting.float_prec_bit);
            int_free(right->value.ptr);

            left->type = FLOAT_V;
            left->value.ptr = res;

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_ul_divide(left->value.chr, right->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                complex_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_ul_divide(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_ul_divide_ul(left->value.chr, right->value.chr, setting.float_prec_bit);

            left->type = FLOAT_V;
            left->value.ptr = res;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_ul_divide_int(left->value.chr, right->value.ptr, setting.float_prec_bit);
            int_free(right->value.ptr);

            left->type = FLOAT_V;
            left->value.ptr = res;

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_ul_divide(left->value.chr, right->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                complex_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            complex_ul_divide(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_ul_divide_ul(left->value.chr, right->value.chr, setting.float_prec_bit);

            left->type = FLOAT_V;
            left->value.ptr = res;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "/", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_modulo(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_modulo(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                int_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_int_modulo(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
            int_free(left->value.ptr);

            if (!right->value.chr)
            {
                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            left->value.ptr = int_set_ull(0);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_modulo_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                float_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_modulo_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_modulo(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                float_free(left->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_modulo_ul(left->value.ptr, 1);

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_modulo_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_ul_modulo(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_ul_modulo(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            left->value.chr = 0;

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            right->value.chr = left->value.chr % right->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_ul_modulo(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);

                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_ul_modulo(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            left->value.chr = 0;

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                runtime_t error = modulo_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            left->value.chr %= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "%", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_quotient(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    int_p res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_quotient(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                int_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_int_quotient(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_quotient_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                float_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            float_quotient_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(left->value.ptr);
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_quotient(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = INT_V;
            left->value.ptr = res;

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_get_int(left->value.ptr);
            float_free(left->value.ptr);

            left->type = INT_V;
            left->value.ptr = res;

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                float_free(left->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_quotient_ul(left->value.ptr, right->value.chr);
            float_free(left->value.ptr);

            left->type = INT_V;
            left->value.ptr = res;

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_ul_quotient(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_ul_quotient(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            left->type = INT_V;
            left->value.ptr = res;

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            right->value.chr = left->value.chr / right->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_ul_quotient(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);

                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            res = float_ul_quotient(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            left->type = INT_V;
            left->value.ptr = res;

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
            {
                runtime_t error = division_by_zero_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            left->value.chr /= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "//", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_power(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_sign(right->value.ptr) < 0)
            {
                float_p res = float_int_power_int(left->value.ptr, right->value.ptr, setting.float_prec_bit);
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                left->type = FLOAT_V;
                left->value.ptr = res;

                return ires_success(left);
            }

            if (!int_fits_ul(right->value.ptr))
            {
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_power(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (int_sign(left->value.ptr) < 0)
            {
                complex_p res = complex_int_power_float(left->value.ptr, right->value.ptr, setting.complex_prec_bit);
                int_free(left->value.ptr);
                float_free(right->value.ptr);

                left->type = COMPLEX_V;
                left->value.ptr = res;

                return ires_success(left);
            }

            float_int_power(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_int_power(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                int_free(left->value.ptr);
                left->value.ptr = int_set_ull(1);
            }

            return ires_success(left);
        case CHAR_V:
            int_power_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            float_power_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            if (float_sign(left->value.ptr) < 0)
            {
                complex_p res = complex_float_power_float(left->value.ptr, right->value.ptr, setting.complex_prec_bit);
                float_free(left->value.ptr);
                float_free(right->value.ptr);

                left->type = COMPLEX_V;
                left->value.ptr = res;

                return ires_success(left);
            }

            float_power(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_float_power(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
            {
                float_free(left->value.ptr);
                left->value.ptr = float_set_ul(1, setting.float_prec_bit);
            }

            return ires_success(left);
        case CHAR_V:
            float_power_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            complex_power_int(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            complex_power_float(left->value.ptr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            complex_power(left->value.ptr, right->value.ptr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            if (!right->value.chr)
            {
                complex_free(left->value.ptr);
                left->value.ptr = complex_set_ul(1, setting.complex_prec_bit);
            }

            return ires_success(left);
        case CHAR_V:
            complex_power_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_sign(right->value.ptr))
            {
                int_free(right->value.ptr);
                right->value.ptr = int_set_ull(1);
            }
            else
            {
                int_free(right->value.ptr);
                right->value.ptr = int_set_ull(left->value.chr);
            }

            return ires_success(right);
        case FLOAT_V:
            if (!float_sign(right->value.ptr))
            {
                float_free(right->value.ptr);
                right->value.ptr = float_set_ul(1, setting.float_prec_bit);
            }
            else
            {
                float_free(right->value.ptr);
                right->value.ptr = float_set_ul(left->value.chr, setting.float_prec_bit);
            }

            return ires_success(right);
        case COMPLEX_V:
            if (complex_is_zero(right->value.ptr))
            {
                complex_free(right->value.ptr);
                right->value.ptr = complex_set_ul(1, setting.complex_prec_bit);
            }
            else
            {
                complex_free(right->value.ptr);
                right->value.ptr = complex_set_ul(left->value.chr, setting.complex_prec_bit);
            }

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
                left->value.chr = 1;

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
                right->value.chr = 1;
            else
                right->value.chr = left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_ul_power(left->value.chr, right->value.ptr);

            return ires_success(right);
        case FLOAT_V:
            float_ul_power(left->value.chr, right->value.ptr);

            return ires_success(right);
        case COMPLEX_V:
            complex_ul_power(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
            if (!right->value.chr)
                left->value.chr = 1;

            return ires_success(left);
        case CHAR_V:
            if (!right->value.chr)
                left->value.chr = 1;
            else
            {
                unsigned char i;
                for (i = 1; i < right->value.chr; i++)
                    left->value.chr *= left->value.chr;
            }

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "**", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_and(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_and(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            int_and_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            int_and_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            right->value.chr &= left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_and_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr &= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "&", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_or(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_or(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            int_or_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            int_or_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            right->value.chr |= left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_or_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr |= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "|", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_xor(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_xor(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            int_xor_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            int_xor_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            right->value.chr ^= left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            int_xor_ul(right->value.ptr, left->value.chr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr ^= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "^", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_lshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_lshift(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            int_lshift_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_ul_lshift(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            right->value.chr <<= left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_ul_lshift(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr <<= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "<<", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_rshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                int_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_rshift(left->value.ptr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            int_rshift_ul(left->value.ptr, right->value.chr);

            return ires_success(left);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_ul_rshift(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            right->value.chr >>= left->value.chr;

            return ires_success(right);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ul(right->value.ptr))
            {
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            int_ul_rshift(left->value.chr, right->value.ptr);

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            left->value.chr >>= right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, ">>", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_equal(value_p left, value_p right)
{
    char res;

    switch (left->type)
    {
    case NULL_V:
        left->type = BOOL_V;
        left->value.chr = right->type == NULL_V;

        return ires_success(left);
    case NONE_V:
        left->type = BOOL_V;
        left->value.chr = right->type == NONE_V;

        return ires_success(left);
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = int_equal(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_equal_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case COMPLEX_V:
            res = complex_equal_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = int_equal_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = float_equal_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_equal(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case COMPLEX_V:
            res = complex_equal_float(right->value.ptr, left->value.ptr);
            float_free(left->value.ptr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = float_equal_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            res = complex_equal_int(left->value.ptr, right->value.ptr);
            complex_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = complex_equal_float(left->value.ptr, right->value.ptr);
            complex_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case COMPLEX_V:
            res = complex_equal(left->value.ptr, right->value.ptr);
            complex_free(left->value.ptr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = complex_equal_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_equal_ul(right->value.ptr, left->value.chr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_equal_ul(right->value.ptr, left->value.chr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            left->value.chr = complex_equal_ul(right->value.ptr, left->value.chr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            left->value.chr ^= ~right->value.chr;

            return ires_success(left);
        case CHAR_V:
            left->value.chr = left->value.chr == right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_equal_ul(right->value.ptr, left->value.chr);
            int_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_equal_ul(right->value.ptr, left->value.chr);
            float_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case COMPLEX_V:
            left->value.chr = complex_equal_ul(right->value.ptr, left->value.chr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = left->value.chr == right->value.chr;

            return ires_success(right);
        case STR_V:
            left->value.chr = str_equal_char(right->value.ptr, left->value.chr);
            str_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case CHAR_V:
            right->value.chr = str_equal_char(left->value.ptr, right->value.chr);
            str_free(left->value.ptr);

            right->type = BOOL_V;

            return ires_success(right);
        case STR_V:
            res = str_equal(left->value.ptr, right->value.ptr);
            str_free(left->value.ptr);
            str_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            res = list_equal(left->value.ptr, right->value.ptr);
            list_free(left->value.ptr);
            list_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case TUPLE_V:
            res = list_equal_tuple(left->value.ptr, right->value.ptr);
            list_free(left->value.ptr);
            tuple_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
            res = list_equal_tuple(right->value.ptr, left->value.ptr);
            tuple_free(left->value.ptr);
            list_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case TUPLE_V:
            res = tuple_equal(left->value.ptr, right->value.ptr);
            tuple_free(left->value.ptr);
            tuple_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case TYPE_V:
        if (right->type == TYPE_V)
        {
            left->type = BOOL_V;
            left->value.chr = left->value.chr == right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    left->type = BOOL_V;
    left->value.chr = 0;

    return ires_success(left);
}

ires_t operate_nequal(value_p left, value_p right)
{
    char res;

    switch (left->type)
    {
    case NULL_V:
        left->type = BOOL_V;
        left->value.chr = right->type != NULL_V;

        return ires_success(left);
    case NONE_V:
        left->type = BOOL_V;
        left->value.chr = right->type != NONE_V;

        return ires_success(left);
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = int_nequal(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_nequal_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case COMPLEX_V:
            res = complex_nequal_int(right->value.ptr, left->value.ptr);
            int_free(left->value.ptr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = int_nequal_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = float_nequal_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_nequal(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case COMPLEX_V:
            res = complex_nequal_float(right->value.ptr, left->value.ptr);
            float_free(left->value.ptr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = float_nequal_ul(left->value.ptr, right->value.chr);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            res = complex_nequal_int(left->value.ptr, right->value.ptr);
            complex_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = complex_nequal_float(left->value.ptr, right->value.ptr);
            complex_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case COMPLEX_V:
            res = complex_nequal(left->value.ptr, right->value.ptr);
            complex_free(left->value.ptr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = complex_nequal_ul(left->value.ptr, right->value.chr);
            complex_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_nequal_ul(right->value.ptr, left->value.chr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_nequal_ul(right->value.ptr, left->value.chr);
            float_free(right->value.ptr);

            return ires_success(left);
        case COMPLEX_V:
            left->value.chr = complex_nequal_ul(right->value.ptr, left->value.chr);
            complex_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
            left->value.chr ^= right->value.chr;

            return ires_success(left);
        case CHAR_V:
            left->value.chr = left->value.chr != right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_nequal_ul(right->value.ptr, left->value.chr);
            int_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_nequal_ul(right->value.ptr, left->value.chr);
            float_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case COMPLEX_V:
            left->value.chr = complex_nequal_ul(right->value.ptr, left->value.chr);
            complex_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = left->value.chr != right->value.chr;

            return ires_success(right);
        case STR_V:
            left->value.chr = str_nequal_char(right->value.ptr, left->value.chr);
            str_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case CHAR_V:
            right->value.chr = str_nequal_char(left->value.ptr, right->value.chr);
            str_free(left->value.ptr);

            right->type = BOOL_V;

            return ires_success(right);
        case STR_V:
            res = str_nequal(left->value.ptr, right->value.ptr);
            str_free(left->value.ptr);
            str_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            res = list_nequal(left->value.ptr, right->value.ptr);
            list_free(left->value.ptr);
            list_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case TUPLE_V:
            res = list_nequal_tuple(left->value.ptr, right->value.ptr);
            list_free(left->value.ptr);
            tuple_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
            res = list_nequal_tuple(right->value.ptr, left->value.ptr);
            tuple_free(left->value.ptr);
            list_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case TUPLE_V:
            res = tuple_nequal(left->value.ptr, right->value.ptr);
            tuple_free(left->value.ptr);
            tuple_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case TYPE_V:
        if (right->type == TYPE_V)
        {
            left->type = BOOL_V;
            left->value.chr = left->value.chr != right->value.chr;

            return ires_success(left);
        }

        break;
    }

    value_free(left);
    value_free(right);

    left->type = BOOL_V;
    left->value.chr = 1;

    return ires_success(left);
}

ires_t operate_less(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = int_less(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_int_less(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = int_less_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = float_less_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_less(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = float_less_ul(left->value.ptr, right->value.chr);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_less(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_less(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            left->value.chr = left->value.chr < right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_less(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_less(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = left->value.chr < right->value.chr;

            return ires_success(right);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "<", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_greater(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = int_greater(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_int_greater(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = int_greater_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = float_greater_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_greater(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = float_greater_ul(left->value.ptr, right->value.chr);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_greater(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_greater(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            left->value.chr = left->value.chr > right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_greater(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_greater(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = left->value.chr > right->value.chr;

            return ires_success(right);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, ">", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_less_eq(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = int_less_equal(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_int_less_equal(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = int_less_equal_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = float_less_equal_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_less_equal(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = float_less_equal_ul(left->value.ptr, right->value.chr);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_less_equal(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_less_equal(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            left->value.chr = left->value.chr <= right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_less_equal(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_less_equal(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = left->value.chr <= right->value.chr;

            return ires_success(right);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "<=", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_greater_eq(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res = int_greater_equal(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_int_greater_equal(left->value.ptr, right->value.ptr);
            int_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = int_greater_equal_ul(left->value.ptr, right->value.chr);
            int_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res = float_greater_equal_int(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case FLOAT_V:
            res = float_greater_equal(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = float_greater_equal_ul(left->value.ptr, right->value.chr);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_greater_equal(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_greater_equal(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            return ires_success(left);
        case BOOL_V:
        case CHAR_V:
            left->value.chr = left->value.chr >= right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->value.chr = int_ul_greater_equal(left->value.chr, right->value.ptr);
            int_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case FLOAT_V:
            left->value.chr = float_ul_greater_equal(left->value.chr, right->value.ptr);
            float_free(right->value.ptr);

            left->type = BOOL_V;

            return ires_success(left);
        case CHAR_V:
            right->type = BOOL_V;
        case BOOL_V:
            right->value.chr = left->value.chr >= right->value.chr;

            return ires_success(right);
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, ">=", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_xor(value_p left, value_p right)
{
    char res = value_is_true(left) ^ value_is_true(right);

    value_free(left);
    value_free(right);

    left->type = BOOL_V;
    left->value.chr = res;

    return ires_success(left);
}

ires_t operate_contain(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char* str;
    char res;

    switch (right->type)
    {
    case STR_V:
        switch (left->type)
        {
        case NULL_V:
            str_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = 1;
            return ires_success(left);
        case NONE_V:
            left->type = BOOL_V;
            left->value.chr = str_contains_str(right->value.ptr, "none");
            str_free(right->value.ptr);

            return ires_success(left);
        case INT_V:
            str = int_get_str(left->value.ptr);
            int_free(left->value.ptr);

            left->type = BOOL_V;
            left->value.chr = str_contains_str(right->value.ptr, str);
            str_free(right->value.ptr);
            free(str);

            return ires_success(left);
        case FLOAT_V:
            str = float_get_str(left->value.ptr, setting.float_prec_show);
            float_free(left->value.ptr);

            left->type = BOOL_V;
            left->value.chr = str_contains_str(right->value.ptr, str);
            str_free(right->value.ptr);
            free(str);

            return ires_success(left);
        case COMPLEX_V:
            str = complex_get_str(left->value.ptr, setting.complex_prec_bit);
            complex_free(left->value.ptr);

            left->type = BOOL_V;
            left->value.chr = str_contains_str(right->value.ptr, str);
            str_free(right->value.ptr);
            free(str);

            return ires_success(left);
        case BOOL_V:
            left->value.chr = str_contains_str(right->value.ptr, left->value.chr ? "true" : "false");
            str_free(right->value.ptr);

            return ires_success(left);
        case CHAR_V:
            left->type = BOOL_V;
            left->value.chr = str_contains_char(right->value.ptr, left->value.chr);
            str_free(right->value.ptr);

            return ires_success(left);
        case STR_V:
            res = str_contains(right->value.ptr, left->value.ptr);
            str_free(left->value.ptr);
            str_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case LIST_V:
        res = list_contains(right->value.ptr, left);
        list_free(right->value.ptr);
        value_free(left);

        left->type = BOOL_V;
        left->value.chr = res;

        return ires_success(left);
    case TUPLE_V:
        res = tuple_contains(right->value.ptr, left);
        tuple_free(right->value.ptr);
        value_free(left);

        left->type = BOOL_V;
        left->value.chr = res;

        return ires_success(left);
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "in", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_is(value_p left, value_p right)
{
    char res;

    if (left->type == TYPE_V)
    {
        res = right->type == left->value.chr;

        value_free(right);
    }
    else if (right->type == TYPE_V)
    {
        res = left->type == right->value.chr;

        value_free(left);
    }
    else
    {
        res = left->type == right->type;

        value_free(left);
        value_free(right);
    }

    left->type = BOOL_V;
    left->value.chr = res;

    return ires_success(left);
}

ires_t operate_are(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char type;
    unsigned long long i;

    switch (left->type)
    {
    case LIST_V:
        type = right->type == TYPE_V ? right->value.chr : right->type;

        for (i = 0; i < ((list_p)left->value.ptr)->size; i++)
            if (((list_p)left->value.ptr)->elements[i].type != type)
            {
                list_free(left->value.ptr);
                value_free(right);

                left->type = BOOL_V;
                left->value.chr = 0;

                return ires_success(left);
            }

        list_free(left->value.ptr);
        value_free(right);

        left->type = BOOL_V;
        left->value.chr = 1;

        return ires_success(left);
    case TUPLE_V:
        type = right->type == TYPE_V ? right->value.chr : right->type;

        for (i = 0; i < ((tuple_p)left->value.ptr)->size; i++)
            if (((tuple_p)left->value.ptr)->elements[i].type != type)
            {
                tuple_free(left->value.ptr);
                value_free(right);

                left->type = BOOL_V;
                left->value.chr = 0;

                return ires_success(left);
            }

        tuple_free(left->value.ptr);
        value_free(right);

        left->type = BOOL_V;
        left->value.chr = 1;

        return ires_success(left);
    }

    char* detail = malloc(81 + value_label_lens[left->type]);
    sprintf(detail, "<%s> is not iterable data structure (iterable data structures: <list> and <tuple>)", value_labels[left->type]);

    runtime_t error = runtime_set(TYPE_E, detail, &left->poss, &left->pose, left->context);
    return ires_fail(&error);
}

ires_t operate_subscript(value_p left, value_p right)
{
    runtime_t error;
    unsigned long long index;
    value_t value;
    char* detail;

    switch (left->type)
    {
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((str_p)left->value.ptr)->size - index;

            if (index >= ((str_p)left->value.ptr)->size)
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            char chr = ((str_p)left->value.ptr)->str[index];

            str_free(left->value.ptr);
            int_free(right->value.ptr);

            left->type = CHAR_V;
            left->value.chr = chr;

            return ires_success(left);
        case BOOL_V:
            right->type = CHAR_V;
        case CHAR_V:
            if (right->value.chr >= ((str_p)left->value.ptr)->size)
            {
                str_free(left->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            right->value.chr = ((str_p)left->value.ptr)->str[right->value.chr];

            str_free(left->value.ptr);

            return ires_success(right);
        }

        str_free(left->value.ptr);
        value_free(right);
        goto index_err;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                list_free(left->value.ptr);
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((list_p)left->value.ptr)->size - index;

            if (index >= ((list_p)left->value.ptr)->size)
            {
                list_free(left->value.ptr);
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            value = ((list_p)left->value.ptr)->elements[index];

            list_free_exception(left->value.ptr, index);
            int_free(right->value.ptr);

            return ires_success(&value);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= ((list_p)left->value.ptr)->size)
            {
                list_free(left->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            value = ((list_p)left->value.ptr)->elements[right->value.chr];

            list_free_exception(left->value.ptr, right->value.chr);

            return ires_success(&value);
        }

        list_free(left->value.ptr);
        value_free(right);
        goto index_err;
    case TUPLE_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                tuple_free(left->value.ptr);
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((tuple_p)left->value.ptr)->size - index;

            if (index >= ((tuple_p)left->value.ptr)->size)
            {
                tuple_free(left->value.ptr);
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            value = ((tuple_p)left->value.ptr)->elements[index];

            tuple_free_exception(left->value.ptr, index);
            int_free(right->value.ptr);

            return ires_success(&value);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= ((tuple_p)left->value.ptr)->size)
            {
                tuple_free(left->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            value = ((tuple_p)left->value.ptr)->elements[right->value.chr];

            tuple_free_exception(left->value.ptr, right->value.chr);

            return ires_success(&value);
        }

        tuple_free(left->value.ptr);
        value_free(right);
        goto index_err;
    }

    detail = malloc(63 + value_label_lens[left->type]);
    sprintf(detail, "<%s> is not iterable (iterable types: <str>, <list> and <tuple>)", value_labels[left->type]);

    error = runtime_set(TYPE_E, detail, &left->poss, &left->pose, left->context);
    return ires_fail(&error);

index_err:
    detail = malloc(47 + value_label_lens[right->type]);
    sprintf(detail, "Index must be <int>, <bool> or <char> (not <%s>)", value_labels[right->type]);

    error = runtime_set(TYPE_E, detail, &right->poss, &right->pose, right->context);
    return ires_fail(&error);
}

ires_t operate_subscript_ptr(value_p left, value_p right, pos_p left_poss, pos_p left_pose)
{
    runtime_t error;
    unsigned long long index;
    void* ptr;
    char* detail;

    switch (left->type)
    {
    case STR_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((str_p)left->value.ptr)->size - index;

            if (index >= ((str_p)left->value.ptr)->size)
            {
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            int_free(right->value.ptr);

            right->type = CHAR_PTR_V;
            right->value.ptr = ((str_p)left->value.ptr)->str + index;

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= ((str_p)left->value.ptr)->size)
            {
                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            right->type = CHAR_PTR_V;
            right->value.ptr = ((str_p)left->value.ptr)->str + right->value.chr;

            return ires_success(right);
        }

        value_free(right);
        goto index_err;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!int_fits_ull(right->value.ptr))
            {
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            index = int_get_ull(right->value.ptr);

            if (int_sign(right->value.ptr) < 0)
                index = ((list_p)left->value.ptr)->size - index;

            if (index >= ((list_p)left->value.ptr)->size)
            {
                int_free(right->value.ptr);

                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            ptr = ((list_p)left->value.ptr)->elements + index;

            int_free(right->value.ptr);

            right->type = NULL_V;
            right->value.ptr = ptr;

            return ires_success(right);
        case BOOL_V:
        case CHAR_V:
            if (right->value.chr >= ((list_p)left->value.ptr)->size)
            {
                error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            right->type = NULL_V;
            right->value.ptr = ((list_p)left->value.ptr)->elements + right->value.chr;

            return ires_success(right);
        }

        value_free(right);
        goto index_err;
    }

    detail = malloc(62 + value_label_lens[left->type]);
    sprintf(detail, "<%s> is not dynamic-iterable (iterable types: <str> and <list>)", value_labels[left->type]);

    error = runtime_set(TYPE_E, detail, left_poss, left_pose, left->context);
    return ires_fail(&error);

index_err:
    detail = malloc(47 + value_label_lens[right->type]);
    sprintf(detail, "Index must be <int>, <bool> or <char> (not <%s>)", value_labels[right->type]);

    error = runtime_set(TYPE_E, detail, &right->poss, &right->pose, right->context);
    return ires_fail(&error);
}

ires_t operate_positive(value_p operand)
{
    unsigned long long size;

    switch (operand->type)
    {
    case NULL_V:
    case NONE_V:
        operand->type = INT_V;
        operand->value.ptr = int_set_ull(0);

        return ires_success(operand);
    case OBJECT_V:
        operand->type = INT_V;
        operand->value.ptr = int_set_ull(1);

        return ires_success(operand);
    case BOOL_V:
    case CHAR_V:
    case TYPE_V:
        operand->type = INT_V;
        operand->value.ptr = int_set_ull(operand->value.chr);

        return ires_success(operand);
    case STR_V:
        size = str_size(operand->value.ptr);
        str_free(operand->value.ptr);

        operand->type = INT_V;
        operand->value.ptr = int_set_ull(size);

        return ires_success(operand);
    case LIST_V:
        size = list_size(operand->value.ptr);
        list_free(operand->value.ptr);

        operand->type = INT_V;
        operand->value.ptr = int_set_ull(size);

        return ires_success(operand);
    case TUPLE_V:
        size = tuple_size(operand->value.ptr);
        str_free(operand->value.ptr);

        operand->type = INT_V;
        operand->value.ptr = int_set_ull(size);

        return ires_success(operand);
    }

    return ires_success(operand);
}

ires_t operate_negate(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    switch (operand->type)
    {
    case INT_V:
        int_negate(operand->value.ptr);

        return ires_success(operand);
    case FLOAT_V:
        float_negate(operand->value.ptr);

        return ires_success(operand);
    case COMPLEX_V:
        complex_negate(operand->value.ptr);

        return ires_success(operand);
    case BOOL_V:
        operand->value.chr = !operand->value.chr;

        return ires_success(operand);
    case CHAR_V:
        operand->value.chr = -operand->value.chr;

        return ires_success(operand);
    case STR_V:
        str_reverse(operand->value.ptr);

        return ires_success(operand);
    case LIST_V:
        list_reverse(operand->value.ptr);

        return ires_success(operand);
    }

    value_free(operand);

    runtime_t error = illegal_operation_unary_error(operand->type, "-", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_not(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    switch (operand->type)
    {
    case INT_V:
        int_not(operand->value.ptr);

        return ires_success(operand);
    case BOOL_V:
        operand->value.chr = !operand->value.chr;

        return ires_success(operand);
    case CHAR_V:
        operand->value.chr = ~operand->value.chr;

        return ires_success(operand);
    }

    value_free(operand);

    runtime_t error = illegal_operation_unary_error(operand->type, "~", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_not(value_p operand)
{
    char res = !value_is_true(operand);
    value_free(operand);

    operand->type = BOOL_V;
    operand->value.chr = res;

    return ires_success(operand);
}

ires_t operate_increment(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    switch (operand->type)
    {
    case INT_V:
        int_add_ul(operand->value.ptr, 1);

        return ires_success(operand);
    case FLOAT_V:
        float_add_ul(operand->value.ptr, 1);

        return ires_success(operand);
    case COMPLEX_V:
        complex_add_ul(operand->value.ptr, 1);

        return ires_success(operand);
    case BOOL_V:
        operand->value.chr = !operand->value.chr;

        return ires_success(operand);
    case CHAR_V:
        operand->value.chr++;

        return ires_success(operand);
    case CHAR_PTR_V:
        (*(char*)operand->value.ptr)++;

        return ires_success(operand);
    }

    runtime_t error = illegal_operation_unary_error(operand->type, "++", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_decrement(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    switch (operand->type)
    {
    case INT_V:
        int_subtract_ul(operand->value.ptr, 1);

        return ires_success(operand);
    case FLOAT_V:
        float_subtract_ul(operand->value.ptr, 1);

        return ires_success(operand);
    case COMPLEX_V:
        complex_subtract_ul(operand->value.ptr, 1);

        return ires_success(operand);
    case BOOL_V:
        operand->value.chr = !operand->value.chr;

        return ires_success(operand);
    case CHAR_V:
        operand->value.chr--;

        return ires_success(operand);
    case CHAR_PTR_V:
        (*(char*)operand->value.ptr)--;

        return ires_success(operand);
    }

    runtime_t error = illegal_operation_unary_error(operand->type, "--", poss, pose, context);
    return ires_fail(&error);
}

char operate_equal_compare(const value_p left, const value_p right)
{
    switch (left->type)
    {
    case NULL_V:
        return right->type == NULL_V;
    case NONE_V:
        return right->type == NONE_V;
    case OBJECT_V:
        return left == right;
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_equal_int(right->value.ptr, left->value.ptr);
        case COMPLEX_V:
            return complex_equal_int(right->value.ptr, left->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return int_equal_ul(left->value.ptr, right->value.chr);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_equal_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_equal(left->value.ptr, right->value.ptr);
        case COMPLEX_V:
            return complex_equal_float(right->value.ptr, left->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return float_equal_ul(left->value.ptr, right->value.chr);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            return complex_equal_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return complex_equal_float(left->value.ptr, right->value.ptr);
        case COMPLEX_V:
            return complex_equal(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return complex_equal_ul(left->value.ptr, right->value.chr);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal_ul(right->value.ptr, left->value.chr);
        case FLOAT_V:
            return float_equal_ul(right->value.ptr, left->value.chr);
        case COMPLEX_V:
            return complex_equal_ul(right->value.ptr, left->value.chr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr == right->value.chr;
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal_ul(right->value.ptr, left->value.chr);
        case FLOAT_V:
            return float_equal_ul(right->value.ptr, left->value.chr);
        case COMPLEX_V:
            return complex_equal_ul(right->value.ptr, left->value.chr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr == right->value.chr;
        case STR_V:
            return str_equal_char(right->value.ptr, left->value.chr);
        }

        break;
    case STR_V:
        switch (right->type)
        {
        case CHAR_V:
            return str_equal_char(left->value.ptr, right->value.chr);
        case STR_V:
            return str_equal(left->value.ptr, right->value.ptr);
        }

        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            return list_equal(left->value.ptr, right->value.ptr);
        case TUPLE_V:
            return list_equal_tuple(left->value.ptr, right->value.ptr);
        }

        break;
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
            return list_equal_tuple(right->value.ptr, left->value.ptr);
        case TUPLE_V:
            return tuple_equal(left->value.ptr, right->value.ptr);
        }

        break;
    }

    return 0;
}

char operate_less_compare(const value_p left, const value_p right)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_less(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_int_less(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return int_less_ul(left->value.ptr, right->value.chr);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_less_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_less(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return float_less_ul(left->value.ptr, right->value.chr);
        }

        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            return int_ul_less(left->value.chr, right->value.ptr);
        case FLOAT_V:
            return float_ul_less(left->value.chr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr < right->value.chr;
        }

        break;
    }

    return 0;
}

char operate_greater_compare(const value_p left, const value_p right)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_greater(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_int_greater(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return int_greater_ul(left->value.ptr, right->value.chr);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_greater_int(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_greater(left->value.ptr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return float_greater_ul(left->value.ptr, right->value.chr);
        }

        break;
    case BOOL_V:
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            return int_ul_greater(left->value.chr, right->value.ptr);
        case FLOAT_V:
            return float_ul_greater(left->value.chr, right->value.ptr);
        case BOOL_V:
        case CHAR_V:
            return left->value.chr > right->value.chr;
        }

        break;
    }

    return 0;
}

void operate_success(value_p left, const value_p right)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            int_add(left->value.ptr, right->value.ptr);
            break;
        case FLOAT_V:
            left->type = FLOAT_V;

            float_p conv = float_set_int(left->value.ptr, setting.float_prec_bit);
            int_free(left->value.ptr);
            left->value.ptr = conv;

            float_add(left->value.ptr, right->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
        case CHAR_PTR_V:
            int_add_ul(left->value.ptr, right->value.chr);
            break;
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            float_add_int(left->value.ptr, right->value.ptr);
            break;
        case FLOAT_V:
            float_add(left->value.ptr, right->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
        case CHAR_PTR_V:
            float_add_ul(left->value.ptr, right->value.chr);
            break;
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            left->type = INT_V;
            left->value.ptr = int_set_ull(left->value.chr);

            int_add(left->value.ptr, right->value.ptr);
            break;
        case FLOAT_V:
            left->type = FLOAT_V;
            left->value.ptr = float_set_ul(left->value.chr, setting.float_prec_bit);

            float_add(left->value.ptr, right->value.ptr);
            break;
        case BOOL_V:
            left->value.chr ^= right->value.chr;
            break;
        case CHAR_V:
            left->type = CHAR_V;
            left->value.chr += right->value.chr;
            break;
        case CHAR_PTR_V:
            left->type = INT_V;
            left->value.ptr = int_set_ull(left->value.chr);

            int_add_ul(left->value.ptr, right->value.chr);
            break;
        }

        break;
    case CHAR_V:
        switch (right->type)
        {
        case INT_V:
            left->type = INT_V;
            left->value.ptr = int_set_ull(left->value.chr);

            int_add(left->value.ptr, right->value.ptr);
            break;
        case FLOAT_V:
            left->type = FLOAT_V;
            left->value.ptr = float_set_ul(left->value.chr, setting.float_prec_bit);

            float_add(left->value.ptr, right->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            left->value.chr += right->value.chr;
            break;
        case CHAR_PTR_V:
            left->type = INT_V;
            left->value.ptr = int_set_ull(left->value.chr);

            int_add_ul(left->value.ptr, right->value.chr);
            break;
        }

        break;
    }
}

char operate_success_type_change(const value_p left, const value_p right)
{
    switch (left->type)
    {
    case INT_V:
        return right->type == FLOAT_V;
    case FLOAT_V:
        return 0;
    case BOOL_V:
        return right->type != BOOL_V;
    case CHAR_V:
        return right->type != BOOL_V && right->type != CHAR_V;
    }

    return 0;
}

char operate_sign(const value_p operand)
{
    switch (operand->type)
    {
    case INT_V:
        return int_sign(operand->value.ptr) < 0;
    case FLOAT_V:
        return float_sign(operand->value.ptr) < 0;
    case BOOL_V:
    case CHAR_V:
    case CHAR_PTR_V:
        return 0;
    }

    return 0;
}

unsigned long long operate_size(const value_p operand)
{
    switch (operand->type)
    {
    case STR_V:
        return ((str_p)operand->value.ptr)->size;
    case LIST_V:
        return ((list_p)operand->value.ptr)->size;
    case TUPLE_V:
        return ((tuple_p)operand->value.ptr)->size;
    }

    return 0;
}

value_t operate_index(const value_p operand, unsigned long long index)
{
    value_t res;

    switch (operand->type)
    {
    case STR_V:
        res.type = CHAR_V;
        res.value.chr = ((str_p)operand->value.ptr)->str[index];

        return res;
    case LIST_V:
        return value_copy(((list_p)operand->value.ptr)->elements + index);
    case TUPLE_V:
        return value_copy(((tuple_p)operand->value.ptr)->elements + index);
    }

    return res;
}

runtime_t illegal_operation_error(unsigned char type1, unsigned char type2, const char* operator, pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(39 + strlen(operator) + value_label_lens[type1] + value_label_lens[type2]);
    sprintf(detail, "Illegal operation (%s) between <%s> and <%s>",
        operator, value_labels[type1], value_labels[type2]);

    return runtime_set(ILLEGAL_OPERATION_E, detail, poss, pose, context);
}

runtime_t illegal_operation_unary_error(unsigned char type, const char* operator, pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(28 + strlen(operator) + value_label_lens[type]);
    sprintf(detail, "Illegal operation (%s) for <%s>", operator, value_labels[type]);

    return runtime_set(ILLEGAL_OPERATION_E, detail, poss, pose, context);
}

runtime_t mem_overflow_error(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(16);
    strcpy(detail, "Memory overflow");

    return runtime_set(MEM_OVERFLOW_E, detail, poss, pose, context);
}

runtime_t out_of_range_error(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(19);
    strcpy(detail, "Index out of range");

    return runtime_set(OUT_OF_RANGE_E, detail, poss, pose, context);
}

runtime_t division_by_zero_error(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(17);
    strcpy(detail, "Division by zero");

    return runtime_set(DIVISION_BY_ZERO_E, detail, poss, pose, context);
}

runtime_t modulo_by_zero_error(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(15);
    strcpy(detail, "Modulo by zero");

    return runtime_set(DIVISION_BY_ZERO_E, detail, poss, pose, context);
}
