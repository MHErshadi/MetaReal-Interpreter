/*/
 * MetaReal version 1.0.0
/*/

#include <debugger/runtime_error.h>
#include <interpreter/value.h>
#include <stdlib.h>
#include <string.h>

runtime_t invalid_access_object(unsigned char type,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(44 + value_label_lens[type]);
    sprintf(detail, "<%s> object can not be accessed like a variable",
        value_labels[type]);

    return runtime_set(ACCESS_E, detail, poss, pose, context);
}

runtime_t invalid_access_statement(const char* statement,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(44 + strlen(statement));
    sprintf(detail, "Result of %s can not be accessed like a variable",
        statement);

    return runtime_set(ACCESS_E, detail, poss, pose, context);
}

runtime_t type_match(unsigned char type1, unsigned char type2,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(59 + value_label_lens[type1] + value_label_lens[type2]);
    sprintf(detail, "Type of variable and type of value do not match (<%s> and <%s>)",
        value_labels[type1], value_labels[type2]);

    return runtime_set(TYPE_E, detail, poss, pose, context);
}

runtime_t const_variable(const char* name,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(26 + strlen(name));
    sprintf(detail, "'%s' is a constant variable",
        name);

    return runtime_set(CONST_E, detail, poss, pose, context);
}

runtime_t type_specified_variable(const char* name, unsigned char type,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(45 + strlen(name) + value_label_lens[type]);
    sprintf(detail, "'%s' is a type-specified variable (type is <%s>)",
        name, value_labels[type]);

    return runtime_set(TYPE_E, detail, poss, pose, context);
}

runtime_t assign_type_specified_variable(unsigned char type1, unsigned char type2,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(60 + value_label_lens[type1] + value_label_lens[type2]);
    sprintf(detail, "Can not assign <%s> to a type-specified variable (type is <%s>)",
        value_labels[type1], value_labels[type2]);

    return runtime_set(TYPE_E, detail, poss, pose, context);
}

runtime_t not_defined(const char* name,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(45 + strlen(name));
    sprintf(detail, "'%s' is not defined", name);

    return runtime_set(NOT_DEFINED_E, detail, poss, pose, context);
}

runtime_t invalid_type(const char* name, const char* types, unsigned char type,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(19 + strlen(name) + strlen(types) + value_label_lens[type]);
    sprintf(detail, "%s must be %s (not <%s>)",
        name, types, value_labels[type]);

    return runtime_set(TYPE_E, detail, poss, pose, context);
}

runtime_t out_of_boundary(const char* name, unsigned long long lbound, unsigned long long rbound,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(35 + strlen(name) + number_length(lbound) + number_length(rbound));
    sprintf(detail, "%s must be between %llu and %llu (inclusive)",
        name, lbound, rbound);

    return runtime_set(INVALID_VALUE_E, detail, poss, pose, context);
}

runtime_t outside_loop(const char* name,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(33 + strlen(name));
    sprintf(detail, "%s statement must be inside a loop",
        name);

    return runtime_set(OUTSIDE_LOOP_E, detail, poss, pose, context);
}

runtime_t invalid_dollar_function(const char* name,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(37 + strlen(name));
    sprintf(detail, "Invalid dollar-function with name '%s'",
        name);

    return runtime_set(INVALID_DOLLAR_FUNCTION_E, detail, poss, pose, context);
}

runtime_t invalid_arg_number(const char* name, unsigned long long size1, unsigned long long size2,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(73 + strlen(name) + number_length(size1) + number_length(size2));
    sprintf(detail, "Invalid argument count for dollar-function '%s' (argument count is %llu, not %llu)",
        name, size1, size2);

    return runtime_set(INVALID_ARG_COUNT_E, detail, poss, pose, context);
}

runtime_t invalid_param_name(const char* name,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(26 + strlen(name));
    sprintf(detail, "Invalid parameter name '%s'", name);

    return runtime_set(INVALID_VALUE_E, detail, poss, pose, context);
}

runtime_t illegal_operation(unsigned char type1, unsigned char type2, const char* operator,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(39 + strlen(operator) + value_label_lens[type1] + value_label_lens[type2]);
    sprintf(detail, "Illegal operation (%s) between <%s> and <%s>",
        operator, value_labels[type1], value_labels[type2]);

    return runtime_set(ILLEGAL_OPERATION_E, detail, poss, pose, context);
}

runtime_t illegal_operation_unary(unsigned char type, const char* operator,
    pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(28 + strlen(operator) + value_label_lens[type]);
    sprintf(detail, "Illegal operation (%s) for <%s>", operator, value_labels[type]);

    return runtime_set(ILLEGAL_OPERATION_E, detail, poss, pose, context);
}

runtime_t mem_overflow(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(16);
    strcpy(detail, "Memory overflow");

    return runtime_set(MEM_OVERFLOW_E, detail, poss, pose, context);
}

runtime_t out_of_range(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(19);
    strcpy(detail, "Index out of range");

    return runtime_set(OUT_OF_RANGE_E, detail, poss, pose, context);
}

runtime_t division_by_zero(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(17);
    strcpy(detail, "Division by zero");

    return runtime_set(DIVISION_BY_ZERO_E, detail, poss, pose, context);
}

runtime_t modulo_by_zero(pos_p poss, pos_p pose, context_p context)
{
    char* detail = malloc(15);
    strcpy(detail, "Modulo by zero");

    return runtime_set(DIVISION_BY_ZERO_E, detail, poss, pose, context);
}
