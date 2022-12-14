/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <complex.h>
#include <str.h>
#include <array/tuple.h>
#include <setting.h>
#include <stdlib.h>
#include <string.h>

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
        case STR_V:
            str = complex_get_str(left->value.ptr, setting.complex_prec_show);

            str_str_concat(str, right->value.ptr);
            free(str);
            float_free(left->value.ptr);

            return ires_success(right);
        }

        break;
    case BOOL_V:
        if (right->type == STR_V)
        {
            str_str_concat(left->value.chr ? "true" : "false", right->value.ptr);

            return ires_success(right);
        }

        break;
    case CHAR_V:
        if (right->type == STR_V)
        {
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
        }

        break;
    case STR_V:
        if (right->type == INT_V)
        {
            if (!int_fits_ull(right->value.ptr))
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            unsigned long long pos = int_get_ull(right->value.ptr);

            if (pos >= ((str_p)left->value.ptr)->size)
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = out_of_range_error(&right->poss, &right->pose, right->context);
                return ires_fail(&error);
            }

            str_remove(left->value.ptr, pos);
            int_free(right->value.ptr);

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
        }

        break;
    case STR_V:
        if (right->type == INT_V)
        {
            if (!int_fits_ull(right->value.ptr))
            {
                str_free(left->value.ptr);
                int_free(right->value.ptr);

                runtime_t error = mem_overflow_error(poss, pose, context);
                return ires_fail(&error);
            }

            unsigned long long count = int_get_ull(right->value.ptr);

            str_repeat(left->value.ptr, count);
            int_free(right->value.ptr);

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

            float_p res = float_int_divide_int(left->value.ptr, right->value.ptr, setting.float_prec_bit);
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

            int_p res = float_quotient(left->value.ptr, right->value.ptr);
            float_free(left->value.ptr);
            float_free(right->value.ptr);

            left->type = INT_V;
            left->value.ptr = res;

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
    if (left->type == INT_V && right->type == INT_V)
    {
        int_and(left->value.ptr, right->value.ptr);
        int_free(right->value.ptr);

        return ires_success(left);
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "&", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_or(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    if (left->type == INT_V && right->type == INT_V)
    {
        int_or(left->value.ptr, right->value.ptr);
        int_free(right->value.ptr);

        return ires_success(left);
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "|", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_xor(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    if (left->type == INT_V && right->type == INT_V)
    {
        int_xor(left->value.ptr, right->value.ptr);
        int_free(right->value.ptr);

        return ires_success(left);
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "^", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_lshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    if (left->type == INT_V && right->type == INT_V)
    {
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
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "<<", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_rshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    if (left->type == INT_V && right->type == INT_V)
    {
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
        }

        break;
    case BOOL_V:
        if (right->type == BOOL_V)
        {
            left->value.chr ^= ~right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        if (right->type == CHAR_V)
        {
            left->type = BOOL_V;
            left->value.chr = left->value.chr == right->value.chr;

            return ires_success(left);
        }

        break;
    case STR_V:
        if (right->type == STR_V)
        {
            res = str_equal(left->value.ptr, right->value.ptr);
            str_free(left->value.ptr);
            str_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case TUPLE_V:
        if (right->type == TUPLE_V)
        {
            res = tuple_equal(left->value.ptr, right->value.ptr);
            tuple_free(left->value.ptr);
            tuple_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

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
        }

        break;
    case BOOL_V:
        if (right->type == BOOL_V)
        {
            left->value.chr ^= right->value.chr;

            return ires_success(left);
        }

        break;
    case CHAR_V:
        if (right->type == CHAR_V)
        {
            left->type = BOOL_V;
            left->value.chr = left->value.chr != right->value.chr;

            return ires_success(left);
        }

        break;
    case STR_V:
        if (right->type == STR_V)
        {
            res = str_nequal(left->value.ptr, right->value.ptr);
            str_free(left->value.ptr);
            str_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

            return ires_success(left);
        }

        break;
    case TUPLE_V:
        if (right->type == TUPLE_V)
        {
            res = tuple_nequal(left->value.ptr, right->value.ptr);
            tuple_free(left->value.ptr);
            tuple_free(right->value.ptr);

            left->type = BOOL_V;
            left->value.chr = res;

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
        }

        break;
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, ">=", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_and(value_p left, value_p right)
{
    char res = value_is_true(left) & value_is_true(right);

    value_free(left);
    value_free(right);

    left->type = BOOL_V;
    left->value.chr = res;

    return ires_success(left);
}

ires_t operate_or(value_p left, value_p right)
{
    char res = value_is_true(left) | value_is_true(right);

    value_free(left);
    value_free(right);

    left->type = BOOL_V;
    left->value.chr = res;

    return ires_success(left);
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
    if (left->type == CHAR_V && right->type == STR_V)
    {
        char res = str_contains(right->value.ptr, left->value.chr);
        str_free(right->value.ptr);

        left->type = BOOL_V;
        left->value.chr = res;

        return ires_success(left);
    }

    value_free(left);
    value_free(right);

    runtime_t error = illegal_operation_error(left->type, right->type, "in", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_type1(value_p left, value_p right, pos_p poss, pos_p pose, context_p context)
{
    char res = left->type == right->type;
    value_free(left);
    value_free(right);

    left->type = BOOL_V;
    left->value.chr = res;

    return ires_success(left);
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
    }

    value_free(operand);

    runtime_t error = illegal_operation_unary_error(operand->type, "-", poss, pose, context);
    return ires_fail(&error);
}

ires_t operate_b_not(value_p operand, pos_p poss, pos_p pose, context_p context)
{
    if (operand->type == INT_V)
    {
        int_not(operand->value.ptr);

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

}

ires_t operate_decrement(value_p operand, pos_p poss, pos_p pose, context_p context)
{

}

char operate_compare(value_p left, value_p right)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_equal(left->value.ptr, right->value.ptr);
        case FLOAT_V:
            return float_equal_int(right->value.ptr, left->value.ptr);
        case COMPLEX_V:
            return complex_equal_int(right->value.ptr, left->value.ptr);
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
        }

        break;
    case BOOL_V:
        if (right->type == BOOL_V)
            return left->value.chr ^ ~right->value.chr;

        break;
    case CHAR_V:
        if (right->type == CHAR_V)
            return left->value.chr == right->value.chr;

        break;
    case STR_V:
        if (right->type == STR_V)
            return str_equal(left->value.ptr, right->value.ptr);

        break;
    case TUPLE_V:
        if (right->type == TUPLE_V)
            return tuple_equal(left->value.ptr, right->value.ptr);

        break;
    }

    return 0;
}
