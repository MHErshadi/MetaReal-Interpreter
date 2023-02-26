/*/
 * MetaReal version 1.0.0
/*/

#include <struct/bi_func.h>
#include <debugger/runtime_error.h>
#include <int.h>
#include <str.h>
#include <array/list.h>
#include <stdlib.h>
#include <string.h>
#include <setting.h>

ires_t bi_print(value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context);
ires_t bi_input(value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context);
ires_t bi_typeof(value_p value);
ires_t bi_exit();
ires_t bi_len(value_p value);
ires_t bi_ptr(value_p value);

ires_t handle_bi_func(unsigned char id, value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context)
{
    switch (id)
    {
    case PRINT_BI:
        return bi_print(args, size, poss, pose, context);
    case INPUT_BI:
        return bi_input(args, size, poss, pose, context);
    case TYPEOF_BI:
        return bi_typeof(*args);
    case EXIT_BI:
        return bi_exit();
    case LEN_BI:
        return bi_len(*args);
    case PTR_BI:
        return bi_ptr(*args);
    default:
        fprintf(stderr, "handle_bi_func function: invalid node type (#%u)\n", id);
        abort();
    }
}

ires_t bi_print(value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context)
{
    if (!size)
    {
        fputc('\n', setting.output);
        return ires_success(NULL);
    }

    if (size == 1)
    {
        value_print(*args, "\n");
        return ires_success(NULL);
    }

    value_print(*args, str_str(args[1]->value.ptr));
    return ires_success(NULL);
}

ires_t bi_input(value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context)
{
    if (!size)
    {
        char* input = malloc(1025);
        fgets(input, 1025, setting.input);

        input[strlen(input) - 1] = '\0';

        return ires_success(value_set1(STR_V, str_set_str(input, strlen(input))));
    }

    if ((*args)->type != STR_V)
        return ires_fail(invalid_type("'prompt'", "<str>", (*args)->type, poss, pose, context));

    fputs(str_str((*args)->value.ptr), setting.output);

    char* input = malloc(1025);
    fgets(input, 1025, setting.input);

    input[strlen(input) - 1] = '\0';

    return ires_success(value_set1(STR_V, str_set_str(input, strlen(input))));
}

ires_t bi_typeof(value_p value)
{
    if (!value)
        return ires_success(value_set2(TYPE_V, 0));

    return ires_success(value_set2(TYPE_V, value->type));
}

ires_t bi_exit()
{
    exit(0);
}

ires_t bi_len(value_p value)
{
    switch (value->type)
    {
    case STR_V:
        return ires_success(value_set1(INT_V, int_set_ull(str_size(value->value.ptr))));
    case LIST_V:
        return ires_success(value_set1(INT_V, int_set_ull(list_size(value->value.ptr))));
    case TUPLE_V:
        return ires_success(value_set1(INT_V, int_set_ull(tuple_size(value->value.ptr))));
    }

    return ires_success(NULL);
}

ires_t bi_ptr(value_p value)
{
    return ires_success(value_set1(INT_V, int_set_ull((unsigned long)value)));
}
