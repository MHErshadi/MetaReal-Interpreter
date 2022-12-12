/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_VALUE__
#define __M_VALUE__

#include "context.h"

enum _value_types_
{
    NULL_V,

    OBJECT_V,
    NONE_V,

    INT_V,
    FLOAT_V,
    COMPLEX_V,

    BOOL_V,

    CHAR_V,

    STR_V,

    LIST_V,
    TUPLE_V,
    DICT_V,
    SET_V
};

static const char* value_labels[13] =
{
    "null",
    "object", "none",
    "int", "float", "complex",
    "bool",
    "char",
    "str",
    "list", "tuple", "dict", "set"
};

static const int value_label_lens[13] =
{
    4,
    6, 4,
    3, 5, 7,
    4,
    4,
    3,
    4, 5, 4, 3
};

value_t value_set1(unsigned char type, void* ptr, pos_p poss, pos_p pose, context_p context);
value_t value_set2(unsigned char type, char chr, pos_p poss, pos_p pose, context_p context);
value_t value_set3(unsigned char type, pos_p poss, pos_p pose, context_p context);

void value_free(value_p value);

void value_label(value_p value, const char* end);

#endif /* __M_VALUE__ */
