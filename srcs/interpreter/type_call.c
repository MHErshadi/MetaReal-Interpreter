/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/type_call.h>

ires_t object_call(void* ptr)
{
    value_t res;

    res.type = OBJECT_V;
    res.value.ptr = ptr;
    res.should_free = 0;

    return ires_success(&res);
}
