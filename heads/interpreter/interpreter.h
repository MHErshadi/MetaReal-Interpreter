/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_INTERPRETER__
#define __M_INTERPRETER__

#include <parser/node.h>
#include <debugger/errlib.h>

#define IRES_RESPONSE_SET(has_error, func_return, loop_continue, loop_break, char_ptr) ((has_error) | (func_return) << 1 | (loop_continue) << 2 | (loop_break) << 3 | (char_ptr) << 4)

#define IRES_HAS_ERROR(x) ((x) & 1)
#define IRES_FUNC_RETURN(x) ((x) >> 1 & 1)
#define IRES_LOOP_CONTINUE(x) ((x) >> 2 & 1)
#define IRES_LOOP_BREAK(x) ((x) >> 3 & 1)
#define IRES_CHAR_PTR(x) ((x) >> 4 & 1)

struct __ires__
{
    value_p value;
    char response;

    runtime_t error;
};
typedef struct __ires__ ires_t;
typedef struct __ires__* ires_p;

ires_t interpret(node_p nodes, context_p context);

ires_t ires_success(value_p value);
ires_t ires_fail(runtime_t error);

#endif /* __M_INTERPRETER__ */
