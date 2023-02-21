/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_VALUE__
#define __M_VALUE__

#include <def.h>

struct __value__
{
    unsigned char type;
    union_value_t value;

    unsigned long long ref;
};
typedef struct __value__ value_t;
typedef struct __value__* value_p;

#define value_copy(v)   \
    do                  \
    {                   \
        if (v)          \
            (v)->ref++; \
    } while (0)

#define value_ref_free(v) \
    do                    \
    {                     \
        if (v)            \
            (v)->ref--;   \
    } while (0)

#define value_free_type(v, vt)         \
    do                                 \
    {                                  \
        if ((v)->ref)                  \
            (v)->ref--;                \
        else                           \
        {                              \
            vt##_free((v)->value.ptr); \
            free(v);                   \
        }                              \
    } while (0)

#define value_free_shell(v) \
    do                      \
    {                       \
        if ((v)->ref)       \
            (v)->ref--;     \
        else                \
            free(v);        \
    } while (0)

enum _value_types_
{
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
    BI_FUNC_V,

    STRUCT_V
};

static const char* value_labels[16] =
{
    "none",
    "object",
    "int", "float", "complex",
    "bool",
    "char",
    "str",
    "list", "tuple", "dict", "set",
    "type",

    "func",
    "built-in func",
    "struct"
};

static const int value_label_lens[16] =
{
    4,
    6,
    3, 5, 7,
    4,
    4,
    3,
    4, 5, 4, 3,
    4,

    4,
    13,
    6
};

value_p value_set1(unsigned char type, void* ptr);
value_p value_set2(unsigned char type, char chr);

void value_free(value_p value);

void value_label(value_p value, const char* end);

char value_is_true(value_p value);

#endif /* __M_VALUE__ */
