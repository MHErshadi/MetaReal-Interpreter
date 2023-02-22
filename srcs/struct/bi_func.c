/*/
 * MetaReal version 1.0.0
/*/

#include <struct/bi_func.h>
#include <debugger/runtime_error.h>
#include <str.h>
#include <stdlib.h>
#include <string.h>
#include <setting.h>

ires_t bi_print(value_p value);
ires_t bi_input(value_p* args, unsigned long long size);
ires_t bi_typeof(value_p value);
ires_t bi_exit();

ires_t handle_bi_func(unsigned char id, value_p* args, unsigned long long size)
{
    switch (id)
    {
    case PRINT_BI:
        return bi_print(*args);
    case INPUT_BI:
        return bi_input(args, size);
    case TYPEOF_BI:
        return bi_typeof(*args);
    case EXIT_BI:
        return bi_exit();
    }
}

ires_t bi_print(value_p value)
{

}

ires_t bi_input(value_p* args, unsigned long long size)
{
    if (!size)
    {
        char* input = malloc(1025);
        fgets(input, 1025, setting.input);

        input[strlen(input) - 1] = '\0';

        return ires_success(value_set1(STR_V, str_set_str(input, strlen(input))));
    }

    //if (value->type != STR_V)
    //    return ires_fail(invalid_type("'prompt'", "<str>", value->type, poss))

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
