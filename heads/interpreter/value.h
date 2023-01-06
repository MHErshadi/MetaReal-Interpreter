/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_VALUE__
#define __M_VALUE__

#include <parser/node.h>

struct __value__
{
    unsigned char type;
    union_value_t value;

    unsigned char should_free;
};
typedef struct __value__ value_t;
typedef struct __value__* value_p;

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

    TYPE_V,

    /* */

    FUNC_V,

    STRCUT_V
};

static const char* value_labels[16] =
{
    "null", "none",
    "object",
    "int", "float", "complex",
    "bool",
    "char",
    "str",
    "list", "tuple", "dict", "set",
    "type",

    "func",
    "struct"
};

static const int value_label_lens[16] =
{
    4, 4,
    6,
    3, 5, 7,
    4,
    4,
    3,
    4, 5, 4, 3,
    4,

    4,
    6
};

value_t value_set1(unsigned char type, void* ptr);
value_t value_set2(unsigned char type, char chr);
value_t value_set3(unsigned char type);

value_t value_copy(const value_p value);

void value_delete(value_p value);
void value_free(value_p value);

void value_label(value_p value, const char* end);

char value_is_true(value_p value);

#endif /* __M_VALUE__ */
