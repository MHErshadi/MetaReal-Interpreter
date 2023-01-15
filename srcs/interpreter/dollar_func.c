/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/dollar_func.h>
#include <debugger/runtime_error.h>
#include <complex.h>
#include <str.h>
#include <stdlib.h>
#include <string.h>

#define check_none_type(a, t)                           \
    {                                                   \
        if (!*args)                                     \
        {                                               \
            *id = 1;                                    \
            return ires_fail(invalid_type(a, t, NONE_V, \
                poss, pose, context));                  \
        }                                               \
    }

ires_t df_param_get(unsigned char* id, const char* param_name, pos_p poss, pos_p pose, context_p context);
void df_float_prec(unsigned long long prec_bit);
void df_float_prec_bit(unsigned long long prec_bit);
void df_float_prec_show(unsigned long long prec_bit);
void df_complex_prec(unsigned long long prec_bit);
void df_complex_prec_bit(unsigned long long prec_bit);
void df_complex_prec_show(unsigned long long prec_bit);

ires_t handle_dollar_func(unsigned char* id, const char* name, value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context)
{
    value_t res;

    if (!strcmp(name, "param_get"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("param_get", 1, size,
                poss, pose, context));

        check_none_type("param_name", "<str>");
        if ((*args)->type != STR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("param_name", "<str>", (*args)->type,
                poss, pose, context));
        }

        return df_param_get(id, str_str((*args)->value.ptr), poss, pose, context);
    }
    if (!strcmp(name, "float_prec"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("float_prec", 1, size,
                poss, pose, context));

        check_none_type("prec_bit", "<int> or <char>");
        if ((*args)->type != INT_V && (*args)->type != CHAR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("prec_bit", "<int> or <char>", (*args)->type,
                poss, pose, context));
        }

        unsigned long long prec_bit = 0;
        switch ((*args)->type)
        {
        case INT_V:
            if (int_sign((*args)->value.ptr) <= 0 || !int_fits_ull((*args)->value.ptr))
            {
                *id = 1;
                return ires_fail(out_of_boundary("prec_bit", 2, -1,
                    poss, pose, context));
            }

            prec_bit = int_get_ull((*args)->value.ptr);
            break;
        case CHAR_V:
            prec_bit = (*args)->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            *id = 1;
            return ires_fail(out_of_boundary("prec_bit", 2, -1,
                poss, pose, context));
        }

        df_float_prec(prec_bit);

        return ires_success(NULL);
    }
    if (!strcmp(name, "float_prec_bit"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("float_prec_bit", 1, size,
                poss, pose, context));

        check_none_type("prec_bit", "<int> or <char>");
        if ((*args)->type != INT_V && (*args)->type != CHAR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("prec_bit", "<int> or <char>", (*args)->type,
                poss, pose, context));
        }

        unsigned long long prec_bit = 0;
        switch ((*args)->type)
        {
        case INT_V:
            if (int_sign((*args)->value.ptr) <= 0 || !int_fits_ull((*args)->value.ptr))
            {
                *id = 1;
                return ires_fail(out_of_boundary("prec_bit", 2, -1,
                    poss, pose, context));
            }

            prec_bit = int_get_ull((*args)->value.ptr);
            break;
        case CHAR_V:
            prec_bit = (*args)->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            *id = 1;
            return ires_fail(out_of_boundary("prec_bit", 2, -1,
                poss, pose, context));
        }

        df_float_prec_bit(prec_bit);

        return ires_success(NULL);
    }
    if (!strcmp(name, "float_prec_show"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("float_prec_show", 1, size,
                poss, pose, context));

        check_none_type("prec_bit", "<int>, <bool> or <char>");
        if ((*args)->type != INT_V && (*args)->type != BOOL_V && (*args)->type != CHAR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("prec_bit", "<int>, <bool> or <char>", (*args)->type,
                poss, pose, context));
        }

        unsigned long long prec_bit = 0;
        switch ((*args)->type)
        {
        case INT_V:
            if (int_sign((*args)->value.ptr) < 0 || !int_fits_ull((*args)->value.ptr))
            {
                *id = 1;
                return ires_fail(out_of_boundary("prec_bit", 0, -1,
                    poss, pose, context));
            }

            prec_bit = int_get_ull((*args)->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = (*args)->value.chr;
            break;
        }

        df_float_prec_show(prec_bit);

        return ires_success(NULL);
    }
    if (!strcmp(name, "complex_prec"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("complex_prec", 1, size,
                poss, pose, context));

        check_none_type("prec_bit", "<int> or <char>");
        if ((*args)->type != INT_V && (*args)->type != CHAR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("prec_bit", "<int> or <char>", (*args)->type,
                poss, pose, context));
        }

        unsigned long long prec_bit = 0;
        switch ((*args)->type)
        {
        case INT_V:
            if (int_sign((*args)->value.ptr) <= 0 || !int_fits_ull((*args)->value.ptr))
            {
                *id = 1;
                return ires_fail(out_of_boundary("prec_bit", 2, -1,
                    poss, pose, context));
            }

            prec_bit = int_get_ull((*args)->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = (*args)->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            *id = 1;
            return ires_fail(out_of_boundary("prec_bit", 2, -1,
                poss, pose, context));
        }

        df_complex_prec(prec_bit);

        return ires_success(NULL);
    }
    if (!strcmp(name, "complex_prec_bit"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("complex_prec_bit", 1, size,
                poss, pose, context));

        check_none_type("prec_bit", "<int> or <char>");
        if ((*args)->type != INT_V && (*args)->type != CHAR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("prec_bit", "<int> or <char>", (*args)->type,
                poss, pose, context));
        }

        unsigned long long prec_bit = 0;
        switch ((*args)->type)
        {
        case INT_V:
            if (int_sign((*args)->value.ptr) <= 0 || !int_fits_ull((*args)->value.ptr))
            {
                *id = 1;
                return ires_fail(out_of_boundary("prec_bit", 2, -1,
                    poss, pose, context));
            }

            prec_bit = int_get_ull((*args)->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = (*args)->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            *id = 1;
            return ires_fail(out_of_boundary("prec_bit", 2, -1,
                poss, pose, context));
        }

        df_complex_prec_bit(prec_bit);

        return ires_success(NULL);
    }
    if (!strcmp(name, "complex_prec_show"))
    {
        if (size != 1)
            return ires_fail(invalid_arg_number("complex_prec_show", 1, size,
                poss, pose, context));

        check_none_type("prec_bit", "<int>, <bool> or <char>");
        if ((*args)->type != INT_V && (*args)->type != BOOL_V && (*args)->type != CHAR_V)
        {
            *id = 1;
            return ires_fail(invalid_type("prec_bit", "<int>, <bool> or <char>", (*args)->type,
                poss, pose, context));
        }

        unsigned long long prec_bit = 0;
        switch ((*args)->type)
        {
        case INT_V:
            if (int_sign((*args)->value.ptr) < 0 || !int_fits_ull((*args)->value.ptr))
            {
                *id = 1;
                return ires_fail(out_of_boundary("prec_bit", 0, -1,
                    poss, pose, context));
            }

            prec_bit = int_get_ull((*args)->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = (*args)->value.chr;
            break;
        }

        df_complex_prec_show(prec_bit);

        return ires_success(NULL);
    }

    return ires_fail(invalid_dollar_function(name,
        poss, pose, context));
}

ires_t df_param_get(unsigned char* id, const char* param_name, pos_p poss, pos_p pose, context_p context)
{
    if (!strcmp(param_name, "float_prec_bit"))
        return ires_success(value_set1(INT_V, int_set_ull(float_prec_bit)));
    if (!strcmp(param_name, "float_prec_show"))
        return ires_success(value_set1(INT_V, int_set_ull(float_prec_show)));
    if (!strcmp(param_name, "complex_prec_bit"))
        return ires_success(value_set1(INT_V, int_set_ull(complex_prec_bit)));
    if (!strcmp(param_name, "complex_prec_show"))
        return ires_success(value_set1(INT_V, int_set_ull(complex_prec_show)));

    *id = 1;
    return ires_fail(invalid_param_name(param_name,
        poss, pose, context));
}

void df_float_prec(unsigned long long prec_bit)
{
    float_prec_bit = prec_bit;
    float_prec_show = prec_bit >> 2;
}

void df_float_prec_bit(unsigned long long prec_bit)
{
    float_prec_bit = prec_bit;
}

void df_float_prec_show(unsigned long long prec_bit)
{
    float_prec_show = prec_bit;
}

void df_complex_prec(unsigned long long prec_bit)
{
    complex_prec_bit = prec_bit;
    complex_prec_show = prec_bit >> 2;
}

void df_complex_prec_bit(unsigned long long prec_bit)
{
    complex_prec_bit = prec_bit;
}

void df_complex_prec_show(unsigned long long prec_bit)
{
    complex_prec_show = prec_bit;
}
