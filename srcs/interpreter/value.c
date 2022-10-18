/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/value.h>
#include <int.h>
#include <float.h>
#include <complex.h>
#include <setting.h>

value_t value_set1(unsigned char type, void* ptr, pos_p poss, pos_p pose)
{
    value_t value;

    value.type = type;
    value.ptr = ptr;
    value.poss = *poss;
    value.pose = *pose;

    return value;
}

value_t value_set2(unsigned char type, pos_p poss, pos_p pose)
{
    value_t value;

    value.type = type;
    value.poss = *poss;
    value.pose = *pose;

    return value;
}

void value_print(value_p value)
{
    switch (value->type)
    {
    case INT_V:
        int_print(setting.output, value->ptr, "\n");
        break;
    case FLOAT_V:
        float_print(setting.output, value->ptr, setting.float_prec_show, "\n");
        break;
    case COMPLEX_V:
        complex_print(setting.output, value->ptr, setting.complex_prec_show, "\n");
        break;
    }
}
