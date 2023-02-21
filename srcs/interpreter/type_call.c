/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/type_call.h>
#include <stdlib.h>

ires_t object_call();

ires_t handle_type_call(unsigned char id, value_p* args)
{
    switch (id)
    {
    case NONE_V:
        return ires_success(NULL);
    case OBJECT_V:
        return object_call();
    }
}

ires_t object_call()
{
    void* ptr = malloc(1);
    free(ptr);

    return ires_success(value_set1(OBJECT_V, ptr));
}
