/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_DICT__
#define __M_DICT__

#include <interpreter/value.h>

struct __dict_element__
{
    value_t key;
    value_t value;

    struct __dict_element__* next;
};
typedef struct __dict_element__ dict_element_t;
typedef struct __dict_element__* dict_element_p;

struct __dict__
{
    dict_element_p elements;
    unsigned long long size;
};
typedef struct __dict__ dict_t;
typedef struct __dict__* dict_p;

#endif /* __M_DICT__ */
