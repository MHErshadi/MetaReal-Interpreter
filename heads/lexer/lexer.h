/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_LEXER__
#define __M_LEXER__

#include "token.h"
#include <debugger/errlib.h>

struct __lres__
{
    token_p tokens;

    char has_error;
    illegal_char_t error;
};
typedef struct __lres__ lres_t;

lres_t lex(const char* code, char terminator);

#endif /* __M_LEXER__ */
