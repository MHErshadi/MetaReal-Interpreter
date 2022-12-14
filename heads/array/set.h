/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_SET__
#define __M_SET__

#include <interpreter/value.h>

struct __set_element__
{
    value_t value;

    struct __set_element__* next;
};
typedef struct __set_element__ set_element_t;
typedef struct __set_element__* set_element_p;

struct __set__
{
    set_element_p elements;
    unsigned long long size;
};
typedef struct __set__ set_t;
typedef struct __set__* set_p;

#endif /* __M_SET__ */
