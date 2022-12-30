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

    TYPE_V,

    /* */

    FUNC_V
};

static const char* value_labels[15] =
{
    "null", "none",
    "object",
    "int", "float", "complex",
    "bool",
    "char",
    "str",
    "list", "tuple", "dict", "set",
    "type",

    "func"
};

static const int value_label_lens[15] =
{
    4, 4,
    6,
    3, 5, 7,
    4,
    4,
    3,
    4, 5, 4, 3,
    4,

    4
};

struct __func_t__
{
    unsigned char type;

    context_t context;
    body_t body;
};
typedef struct __func_t__ func_t;
typedef struct __func_t__* func_p;

value_t value_set1(unsigned char type, void* ptr, pos_p poss, pos_p pose, context_p context);
value_t value_set2(unsigned char type, char chr, pos_p poss, pos_p pose, context_p context);
value_t value_set3(unsigned char type, pos_p poss, pos_p pose, context_p context);

value_t value_copy(const value_p value);

void value_delete(value_p value);
void value_free(value_p value);

void value_label(value_p value, const char* end);

char value_is_true(value_p value);

/* */

func_p func_set(unsigned char type, context_p context, body_p body);

void func_free(func_p func);

#endif /* __M_VALUE__ */
