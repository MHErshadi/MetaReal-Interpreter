/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/type_call.h>
#include <stdlib.h>

ires_t object_call(void* ptr)
{
    return ires_success(value_set1(OBJECT_V, ptr));
}
