/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/dollar_func.h>
#include <setting.h>
#include <int.h>
#include <str.h>
#include <stdlib.h>
#include <string.h>

ires_t df_param_get(const char* param_name, pos_p poss, pos_p pose, context_p context);
void df_float_prec(unsigned long long prec_bit);
void df_float_prec_bit(unsigned long long prec_bit);
void df_float_prec_show(unsigned long long prec_bit);
void df_complex_prec(unsigned long long prec_bit);
void df_complex_prec_bit(unsigned long long prec_bit);
void df_complex_prec_show(unsigned long long prec_bit);

runtime_t invalid_arg_number_error(const char* name, unsigned long long size1, unsigned long long size2, pos_p poss, pos_p pose, context_p context);
runtime_t type_error(const char* fname, const char* name, const char* type1, unsigned char type2, pos_p poss, pos_p pose, context_p context);
runtime_t boundary_error(unsigned long long lbound, unsigned long long rbound, const char* fname, const char* name, pos_p poss, pos_p pose, context_p context);

ires_t handle_dollar_func(const char* name, value_p args, unsigned long long size, pos_p poss, pos_p pose, context_p context)
{
    value_t res;

    if (!strcmp(name, "param_get"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("param_get", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != STR_V)
        {
            runtime_t error = type_error("param_get", "param_name", "<str>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        return df_param_get(((str_p)args->value.ptr)->str, &args->poss, &args->pose, args->context);
    }
    if (!strcmp(name, "float_prec"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("float_prec", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != INT_V && args->type != BOOL_V && args->type != CHAR_V)
        {
            runtime_t error = type_error("float_prec", "prec_bit", "<int>, <bool> or <char>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        unsigned long long prec_bit = 0;
        switch (args->type)
        {
        case INT_V:
            if (args->type == INT_V && (int_sign(args->value.ptr) <= 0 || !int_fits_ull(args->value.ptr)))
            {
                runtime_t error = boundary_error(2, -1, "float_prec", "prec_bit", &args->poss, &args->pose, args->context);
                return ires_fail(&error);
            }

            prec_bit = int_get_ull(args->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = args->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            runtime_t error = boundary_error(2, -1, "float_prec", "prec_bit", &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        df_float_prec(prec_bit);

        res.type = NULL_V;
        return ires_success(&res);
    }
    if (!strcmp(name, "float_prec_bit"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("float_prec_bit", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != INT_V && args->type != BOOL_V && args->type != CHAR_V)
        {
            runtime_t error = type_error("float_prec_bit", "prec_bit", "<int>, <bool> or <char>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        unsigned long long prec_bit = 0;
        switch (args->type)
        {
        case INT_V:
            if (args->type == INT_V && (int_sign(args->value.ptr) <= 0 || !int_fits_ull(args->value.ptr)))
            {
                runtime_t error = boundary_error(2, -1, "float_prec_bit", "prec_bit", &args->poss, &args->pose, args->context);
                return ires_fail(&error);
            }

            prec_bit = int_get_ull(args->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = args->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            runtime_t error = boundary_error(2, -1, "float_prec_bit", "prec_bit", &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        df_float_prec_bit(prec_bit);

        res.type = NULL_V;
        return ires_success(&res);
    }
    if (!strcmp(name, "float_prec_show"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("float_prec_show", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != INT_V && args->type != BOOL_V && args->type != CHAR_V)
        {
            runtime_t error = type_error("float_prec_show", "prec_bit", "<int>, <bool> or <char>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        unsigned long long prec_bit = 0;
        switch (args->type)
        {
        case INT_V:
            if (args->type == INT_V && (int_sign(args->value.ptr) < 0 || !int_fits_ull(args->value.ptr)))
            {
                runtime_t error = boundary_error(0, -1, "float_prec_show", "prec_bit", &args->poss, &args->pose, args->context);
                return ires_fail(&error);
            }

            prec_bit = int_get_ull(args->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = args->value.chr;
            break;
        }

        df_float_prec_show(prec_bit);

        res.type = NULL_V;
        return ires_success(&res);
    }
    if (!strcmp(name, "complex_prec"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("complex_prec", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != INT_V && args->type != BOOL_V && args->type != CHAR_V)
        {
            runtime_t error = type_error("complex_prec", "prec_bit", "<int>, <bool> or <char>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        unsigned long long prec_bit = 0;
        switch (args->type)
        {
        case INT_V:
            if (args->type == INT_V && (int_sign(args->value.ptr) <= 0 || !int_fits_ull(args->value.ptr)))
            {
                runtime_t error = boundary_error(2, -1, "complex_prec", "prec_bit", &args->poss, &args->pose, args->context);
                return ires_fail(&error);
            }

            prec_bit = int_get_ull(args->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = args->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            runtime_t error = boundary_error(2, -1, "complex_prec", "prec_bit", &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        df_complex_prec(prec_bit);

        res.type = NULL_V;
        return ires_success(&res);
    }
    if (!strcmp(name, "complex_prec_bit"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("complex_prec_bit", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != INT_V && args->type != BOOL_V && args->type != CHAR_V)
        {
            runtime_t error = type_error("complex_prec_bit", "prec_bit", "<int>, <bool> or <char>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        unsigned long long prec_bit = 0;
        switch (args->type)
        {
        case INT_V:
            if (args->type == INT_V && (int_sign(args->value.ptr) <= 0 || !int_fits_ull(args->value.ptr)))
            {
                runtime_t error = boundary_error(2, -1, "complex_prec_bit", "prec_bit", &args->poss, &args->pose, args->context);
                return ires_fail(&error);
            }

            prec_bit = int_get_ull(args->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = args->value.chr;
            break;
        }

        if (prec_bit <= 1)
        {
            runtime_t error = boundary_error(2, -1, "complex_prec_bit", "prec_bit", &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        df_complex_prec_bit(prec_bit);

        res.type = NULL_V;
        return ires_success(&res);
    }
    if (!strcmp(name, "complex_prec_show"))
    {
        if (size != 1)
        {
            runtime_t error = invalid_arg_number_error("complex_prec_show", 1, size, poss, pose, context);
            return ires_fail(&error);
        }

        if (args->type != INT_V && args->type != BOOL_V && args->type != CHAR_V)
        {
            runtime_t error = type_error("complex_prec_show", "prec_bit", "<int>, <bool> or <char>",
                args->type, &args->poss, &args->pose, args->context);
            return ires_fail(&error);
        }

        unsigned long long prec_bit = 0;
        switch (args->type)
        {
        case INT_V:
            if (args->type == INT_V && (int_sign(args->value.ptr) < 0 || !int_fits_ull(args->value.ptr)))
            {
                runtime_t error = boundary_error(0, -1, "complex_prec_show", "prec_bit", &args->poss, &args->pose, args->context);
                return ires_fail(&error);
            }

            prec_bit = int_get_ull(args->value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            prec_bit = args->value.chr;
            break;
        }

        df_complex_prec_show(prec_bit);

        res.type = NULL_V;
        return ires_success(&res);
    }

    char* detail = malloc(37 + strlen(name));
    sprintf(detail, "Invalid dollar-function with name '%s'", name);

    runtime_t error = runtime_set(INVALID_DOLLAR_FUNCTION_E, detail, poss, pose, context);
    return ires_fail(&error);
}

ires_t df_param_get(const char* param_name, pos_p poss, pos_p pose, context_p context)
{
    value_t res;

    if (!strcmp(param_name, "float_prec_bit"))
    {
        res.type = INT_V;
        res.value.ptr = int_set_ull(setting.float_prec_bit);

        return ires_success(&res);
    }
    if (!strcmp(param_name, "float_prec_show"))
    {
        res.type = INT_V;
        res.value.ptr = int_set_ull(setting.float_prec_show);

        return ires_success(&res);
    }
    if (!strcmp(param_name, "complex_prec_bit"))
    {
        res.type = INT_V;
        res.value.ptr = int_set_ull(setting.complex_prec_bit);

        return ires_success(&res);
    }
    if (!strcmp(param_name, "complex_prec_show"))
    {
        res.type = INT_V;
        res.value.ptr = int_set_ull(setting.complex_prec_show);

        return ires_success(&res);
    }

    char* detail = malloc(57 + strlen(param_name));
    sprintf(detail, "Invalid parameter name '%s' in dollar-function 'param_get'", param_name);

    runtime_t error = runtime_set(INVALID_VALUE_E, detail, poss, pose, context);
    return ires_fail(&error);
}

void df_float_prec(unsigned long long prec_bit)
{
    setting.float_prec_bit = prec_bit;
    setting.float_prec_show = prec_bit >> 2;
}

void df_float_prec_bit(unsigned long long prec_bit)
{
    setting.float_prec_bit = prec_bit;
}

void df_float_prec_show(unsigned long long prec_bit)
{
    setting.float_prec_show = prec_bit;
}

void df_complex_prec(unsigned long long prec_bit)
{
    setting.complex_prec_bit = prec_bit;
    setting.complex_prec_show = prec_bit >> 2;
}

void df_complex_prec_bit(unsigned long long prec_bit)
{
    setting.complex_prec_bit = prec_bit;
}

void df_complex_prec_show(unsigned long long prec_bit)
{
    setting.complex_prec_show = prec_bit;
}


runtime_t invalid_arg_number_error(const char* name, unsigned long long size1, unsigned long long size2, pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(73 + strlen(name) + number_length(size1) + number_length(size2));
    sprintf(detail, "Invalid argument count for dollar-function '%s' (argument count is %llu, not %llu)",
        name, size1, size2);

    return runtime_set(INVALID_ARG_COUNT_E, detail, poss, pose, context);
}

runtime_t type_error(const char* fname, const char* name, const char* type1, unsigned char type2, pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(52 + strlen(fname) + strlen(name) + strlen(type1) + value_label_lens[type2]);
    sprintf(detail, "Argument '%s' in dollar-function '%s' must be %s (not <%s>)",
        name, fname, type1, value_labels[type2]);

    return runtime_set(TYPE_E, detail, poss, pose, context);
}

runtime_t boundary_error(unsigned long long lbound, unsigned long long rbound, const char* fname, const char* name, pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(89 + strlen(fname) + strlen(name) + number_length(lbound) + number_length(rbound));
    sprintf(detail, "Argument '%s' in dollar-function '%s' must be between %llu and %llu (inclusive)",
        name, fname, lbound, rbound);

    return runtime_set(BOUNDARY_E, detail, poss, pose, context);
}
