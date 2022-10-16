/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/value.h>

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
