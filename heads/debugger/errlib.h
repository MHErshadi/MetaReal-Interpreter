/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_ERRLIB__
#define __M_ERRLIB__

#include "pos.h"

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
};
typedef struct __runtime__ runtime_t;
typedef struct __runtime__* runtime_p;

enum _runtime_types_
{
    DIV_BY_ZERO_E
};

illegal_char_t illegal_char_set(char chr, pos_t pos);
void illegal_char_print(illegal_char_p error, const char* code, unsigned long long size, const char* fname);

invalid_syntax_t invalid_syntax_set(const char* detail, pos_t poss, pos_t pose);
void invalid_syntax_print(invalid_syntax_p error, const char* code, unsigned long long size, const char* fname);

runtime_t runtime_set(unsigned char type, char* detail, pos_t poss, pos_t pose);
void runtime_print(runtime_p error, const char* code, unsigned long long size);

#endif /* __M_ERRLIB__ */
