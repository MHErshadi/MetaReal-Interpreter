/*/
 * MetaReal version 1.0.0
/*/

#ifndef __TOKEN__
#define __TOKEN__

#include <debugger/pos.h>

struct __token__
{
    unsigned long long type;
    const char* value;

    pos_t poss;
    pos_t pose;
};
typedef struct __token__ token_t;
typedef struct __token__* token_p;

enum _token_types_
{
    EOF_T,         // end of file
    NEWLINE_T,     // new line
    SEMICOLON_T,   // ;

    /* */

    IDENTIFIER_T,  // name

    /* */

    INT_T,         // 103
    FLOAT_T,       // 8.5
    COMPLEX_T,     // 45i

    STR_T,         // "hello world"

    ELLIPSIS_T,    // ...

    /* */

    PLUS_T,        // +
    MINUS_T,       // -
    MULTIPLY_T,    // *
    DIVIDE_T,      // /
    MODULO_T,      // %
    QUOTIENT_T,    // //
    POWER_T,       // **

    B_AND_T,       // &
    B_OR_T,        // |
    B_XOR_T,       // ^
    LSHIFT_T,      // <<
    RSHIFT_T,      // >>
    B_NOT_T,       // ~

    EQUAL_T,       // ==
    NEQUAL_T,      // !=
    LESS_T,        // <
    GREATER_T,     // >
    LESS_EQ_T,     // <=
    GREATER_EQ_T,  // >=

    AND_T,         // &&
    OR_T,          // ||
    XOR_T,         // ^^
    NOT_T,         // !

    /* */

    ASSIGN_T,      // =

    PLUS_EQ_T,     // +=
    MINUS_EQ_T,    // -=
    MULTIPLY_EQ_T, // *=
    DIVIDE_EQ_T,   // /=
    MODULO_EQ_T,   // %=
    QUOTIENT_EQ_T, // //=
    POWER_EQ_T,    // **=

    B_AND_EQ_T,    // &=
    B_OR_EQ_T,     // |=
    B_XOR_EQ_T,    // ^=

    /* */

    LPAREN_T,      // (
    RPAREN_T,      // )

    LSQUARE_T,     // [
    RSQUARE_T,     // ]

    LCURLY_T,      // {
    RCURLY_T,      // }

    /* */

    QUESTION_T,    // ?
    COLON_T,       // :

    DOT_T,         // .

    COMMA_T,       // ,

    DOLLAR_T,      // $

    /* */

    PRIVATE_TK,    // private
    PUBLIC_TK,     // public

    LOCAL_TK,      // local
    GLOBAL_TK,     // global

    CONST_TK,      // const
    STATIC_TK,     // static

    /* */

    VAR_TK,        // var
    FUNC_TK,       // func
    CLASS_TK,      // class
    STRUCT_TK,     // struct
    ENUM_TK,       // enum

    /* */

    IF_TK,         // if
    ELIF_TK,       // elif
    ELSE_TK,       // else

    SWITCH_TK,     // switch
    CASE_TK,       // case
    DEFAULT_TK,    // default

    FOR_TK,        // for
    TO_TK,         // to
    STEP_TK,       // step

    DO_TK,         // do
    WHILE_TK,      // while

    TRY_TK,        // try
    EXCEPT_TK,     // except
    FINALLY_TK,    // finally

    IMPORT_TK,     // import
    INCLUDE_TK,    // include

    /* */

    RETURN_TK,     // return

    CONTINUE_TK,   // continue
    BREAK_TK,      // break

    /* */

    AND_TK,        // and
    OR_TK,         // or
    XOR_TK,        // xor
    NOT_TK,        // not

    IN_TK,         // in

    IS_TK,         // is
    ARE_TK,        // are

    /* */

    NONE_TK,       // none

    TRUE_TK,       // true
    FALSE_TK,      // false

    /* */

    OBJECT_TT,     // object

    INT_TT,        // int
    FLOAT_TT,      // float
    COMPLEX_TT,    // complex

    CHAR_TT,       // char

    BOOL_TT,       // bool

    STR_TT,        // str

    LIST_TT,       // list
    TUPLE_TT,      // tuple
    DICT_TT,       // dict
    SET_TT         // set
};

#define KEYWORDS_PAD PRIVATE_TK
#define TYPES_PAD OBJECT_TT

#endif /* __TOKEN__ */
