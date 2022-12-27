/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_ERRLIB__
#define __M_ERRLIB__

#include <interpreter/context.h>

struct __illegal_char__
{
    char chr;

    pos_t pos;
};
typedef struct __illegal_char__ illegal_char_t;
typedef struct __illegal_char__* illegal_char_p;

struct __invalid_syntax__
{
    const char* detail;

    pos_t poss;
    pos_t pose;
};
typedef struct __invalid_syntax__ invalid_syntax_t;
typedef struct __invalid_syntax__* invalid_syntax_p;

struct __runtime__
{
    unsigned char type;
    char* detail;

    pos_t poss;
    pos_t pose;

    context_p context;
};
typedef struct __runtime__ runtime_t;
typedef struct __runtime__* runtime_p;

#define ERROR_CODE_COUNT 12

enum _runtime_types_
{
    TYPE_E,
    ILLEGAL_OPERATION_E,
    MEM_OVERFLOW_E,
    OUT_OF_RANGE_E,
    DIVISION_BY_ZERO_E,
    NOT_DEFINED_E,
    CONST_E,
    ACCESS_E,
    OUTSIDE_LOOP_E,
    INVALID_ARG_COUNT_E,
    BOUNDARY_E,
    INVALID_DOLLAR_FUNCTION_E,
    INVALID_VALUE_E
};

static const char* runtime_labels[ERROR_CODE_COUNT + 1] =
{
    "TypeError",
    "IllegalOpError",
    "MemOverflowError",
    "OutRangeError",
    "DivByZeroError",
    "NotDefinedError",
    "ConstError",
    "AccessError",
    "OutsideLoopError",
    "InvalidArgCountError",
    "BoundaryError",
    "InvalidDollarFunctionError",
    "InvalidValueError"
};

illegal_char_t illegal_char_set(char chr, pos_p pos);
void illegal_char_print(illegal_char_p error, const char* code, unsigned long long size, const char* fname);

invalid_syntax_t invalid_syntax_set(const char* detail, pos_p poss, pos_p pose);
void invalid_syntax_print(invalid_syntax_p error, const char* code, unsigned long long size, const char* fname);

runtime_t runtime_set(unsigned char type, char* detail, pos_p poss, pos_p pose, context_p context);
void runtime_print(runtime_p error, const char* code, unsigned long long size);

unsigned char number_length(unsigned long long number);

#endif /* __M_ERRLIB__ */
