/*/
 * MetaReal version 1.0.0
/*/

#include <struct/bi_func.h>

ires_t bi_typeof(value_p value);

ires_t handle_bi_func(unsigned char id, value_p* args)
{
    switch (id)
    {
    case PRINT_BI:
    case INPUT_BI:
    case TYPEOF_BI:
        return bi_typeof(*args);
    }
}

ires_t bi_typeof(value_p value)
{
    if (!value)
        return ires_success(value_set2(TYPE_V, 0));

    return ires_success(value_set2(TYPE_V, value->type));
}
