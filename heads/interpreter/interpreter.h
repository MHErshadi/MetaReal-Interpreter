/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_INTERPRETER__
#define __M_INTERPRETER__

#include "value.h"
#include <parser/node.h>
#include <debugger/errlib.h>

struct __ires__
{
    value_t value;

    char has_error;
    runtime_t error;
};
typedef struct __ires__ ires_t;
typedef struct __ires__* ires_p;

ires_t interpret(node_p nodes, context_p context);

ires_t ires_success(value_p value);
ires_t ires_fail(runtime_p error);

#endif /* __M_INTERPRETER__ */
