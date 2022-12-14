/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_LIST__
#define __M_LIST__

#include <interpreter/value.h>

struct __list_element__
{
    value_t value;

    struct __list_element__* next;
};
typedef struct __list_element__ list_element_t;
typedef struct __list_element__* list_element_p;

struct __list__
{
    list_element_p element;
    unsigned long long size;
};
typedef struct __list__ list_t;
typedef struct __list__* list_p;

#endif /* __M_LIST__ */
