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
    TYPEOF_BI
};

static const char* bi_func_names[3] =
{
    "print",
    "input",
    "typeof"
};

static const unsigned long long bi_func_min_sizes[3] =
{
    1,
    0,
    1
};

static const unsigned long long bi_func_max_sizes[3] =
{
    1,
    1,
    1
};

ires_t handle_bi_func(unsigned char id, value_p* args);

#endif /* __M_BI_FUNCTION__ */
