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
    EXIT_BI,
    CLEAR_BI,
    LEN_BI,
    PTR_BI,
    EVAL_BI
};

static const char* bi_func_names[8] =
{
    "print",
    "input",
    "typeof",
    "exit",
    "clear",
    "len",
    "ptr",
    "eval"
};

static const unsigned long long bi_func_min_sizes[8] = { 0, 0, 1, 0, 0, 1, 1, 1 };
static const unsigned long long bi_func_max_sizes[8] = { 2, 1, 1, 0, 0, 1, 1, 1 };

ires_t handle_bi_func(unsigned char id, value_p* args, unsigned long long size,
    pos_p poss, pos_p pose, context_p context);

#endif /* __M_BI_FUNCTION__ */
