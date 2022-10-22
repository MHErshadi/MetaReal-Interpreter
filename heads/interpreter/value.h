/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_VALUE__
#define __M_VALUE__

#include <debugger/pos.h>

enum _value_types_
{
    NULL_V,

    INT_V,
    FLOAT_V,
    COMPLEX_V
};

struct __value__
{
    unsigned char type;
    void* ptr;

    pos_t poss;
    pos_t pose;
};
typedef struct __value__ value_t;
typedef struct __value__* value_p;

value_t value_set1(unsigned char type, void* ptr, pos_p poss, pos_p pose);
value_t value_set2(unsigned char type, pos_p poss, pos_p pose);

void value_free(value_p value);

void value_print(value_p value);

#endif /* __M_VALUE__ */
