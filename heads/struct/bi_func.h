/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_BI_FUNCTION__
#define __M_BI_FUNCTION__

#include <interpreter/interpreter.h>

enum _bi_func_types_
{
    PRINT_BI,
    INPUT_BI,
    TYPEOF_BI,
    EXIT_BI
};

static const char* bi_func_names[4] =
{
    "print",
    "input",
    "typeof",
    "exit"
};

static const unsigned long long bi_func_min_sizes[4] = { 1, 0, 1, 0 };
static const unsigned long long bi_func_max_sizes[4] = { 1, 1, 1, 0 };

ires_t handle_bi_func(unsigned char id, value_p* args, unsigned long long size);

#endif /* __M_BI_FUNCTION__ */
