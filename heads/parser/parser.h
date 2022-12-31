/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_PARSER__
#define __M_PARSER__

#include <lexer/token.h>
#include <debugger/errlib.h>

struct __pres__
{
    node_p nodes;

    char has_error;
    invalid_syntax_t error;
};
typedef struct __pres__ pres_t;
typedef struct __pres__* pres_p;

pres_t parse(token_p tokens);

#endif /* __M_PARSER__ */
