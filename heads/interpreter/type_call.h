/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_TYPE_CALL__
#define __M_TYPE_CALL__

#include "interpreter.h"

static const unsigned long long type_call_min_sizes[13] = { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
static const unsigned long long type_call_max_sizes[13] = { 0, 0, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, 0 };

ires_t handle_type_call(unsigned char id, value_p* args, unsigned long long size);

#endif /* __M_TYPE_CALL__ */
