/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_VALUE__
#define __M_VALUE__

#include "context.h"

enum _value_types_
{
    NULL_V,
    NONE_V,

    OBJECT_V,

    INT_V,
    FLOAT_V,
    COMPLEX_V,

    BOOL_V,

    CHAR_V,

    STR_V,

    LIST_V,
    TUPLE_V,
    DICT_V,
    SET_V,

    TYPE_V
};

static const char* value_labels[14] =
{
    "null", "none",
    "object",
    "int", "float", "complex",
    "bool",
    "char",
    "str",
    "list", "tuple", "dict", "set",
    "type"
};

static const int value_label_lens[14] =
{
    4, 4,
    6,
    3, 5, 7,
    4,
    4,
    3,
    4, 5, 4, 3,
    4
};

value_t value_set1(unsigned char type, void* ptr, pos_p poss, pos_p pose, context_p context);
value_t value_set2(unsigned char type, char chr, pos_p poss, pos_p pose, context_p context);
value_t value_set3(unsigned char type, pos_p poss, pos_p pose, context_p context);

value_t value_copy(const value_p value);

void value_free(value_p value);

void value_label(value_p value, const char* end);

char value_is_true(value_p value);

#endif /* __M_VALUE__ */
