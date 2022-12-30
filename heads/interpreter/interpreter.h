/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_INTERPRETER__
#define __M_INTERPRETER__

#include "value.h"
#include <parser/node.h>
#include <debugger/errlib.h>

#define IRES_RESPONSE_SET(has_error, loop_continue, loop_break, char_ptr) ((has_error) | (loop_continue) << 1 | (loop_break) << 2 | (char_ptr) << 3)
#define IRES_RESPONSE_MASK 0b110

#define IRES_HAS_ERROR(x) ((x) & 1)
#define IRES_LOOP_CONTINUE(x) ((x) >> 1 & 1)
#define IRES_LOOP_BREAK(x) ((x) >> 2 & 1)
#define IRES_CHAR_PTR(x) ((x) >> 3 & 1)

struct __ires__
{
    value_t value;
    char response;

    runtime_t error;
};
typedef struct __ires__ ires_t;
typedef struct __ires__* ires_p;

ires_t interpret(node_p nodes, context_p context);

ires_t ires_success(value_p value);
ires_t ires_fail(runtime_t error);

#endif /* __M_INTERPRETER__ */
