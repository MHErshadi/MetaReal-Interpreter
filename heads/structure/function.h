/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_FUNCTION__
#define __M_FUNCTION__

#include <interpreter/context.h>
#include <parser/node.h>

struct __arg_value_t__
{
    value_p value;
    unsigned long long index;
};
typedef struct __arg_value_t__ arg_value_t;
typedef struct __arg_value_t__* arg_value_p;

struct __func_t__
{
    unsigned char type;

    unsigned long long min_size;
    unsigned long long max_size;

    arg_value_p args;
    unsigned long long size;

    context_t context;
    body_t body;
};
typedef struct __func_t__ func_t;
typedef struct __func_t__* func_p;

#define func_context(f) (((func_p)f)->context)
#define func_name(f) (((func_p)f)->context.name)

/* */

func_p func_set(unsigned char type, unsigned long long min_size, unsigned long long max_size,
    arg_value_p args, unsigned long long size,
    context_p context, body_p body);

void func_free(func_p func);

/* */

void arg_value_free(arg_value_p args, unsigned long long size);

/* */

void func_table_free(table_p table, unsigned long long arg_size);
void func_table_free_args(table_p table, unsigned long long arg_size);

#endif /* __M_FUNCTION__ */
