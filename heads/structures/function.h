/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_FUNCTION__
#define __M_FUNCTION__

#include <interpreter/context.h>

struct __func_t__
{
    unsigned char type;

    context_t context;
    body_t body;
};
typedef struct __func_t__ func_t;
typedef struct __func_t__* func_p;

func_p func_set(unsigned char type, context_p context, body_p body);

func_p func_copy(const func_p func);

void func_free(func_p func);

#endif /* __M_FUNCTION__ */
