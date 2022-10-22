/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_TOKEN__
#define __M_TOKEN__

#include <debugger/pos.h>
#include <stdio.h>

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

    CHAR_T,        // 'A'

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

    /* */

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

    INCREMENT_T,   // ++
    DECREMENT_T,   // --

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
    LSHIFT_EQ_T,   // <<=
    RSHIFT_EQ_T,   // >>=

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

struct __token__
{
    unsigned long long type;

    char* value;
    unsigned long long size;

    pos_t poss;
    pos_t pose;
};
typedef struct __token__ token_t;
typedef struct __token__* token_p;

static const char* token_labels[109] =
{
    "EOF", "NEWLINE", "SEMICOLON",

    "IDENTIFIER",

    "INT", "FLOAT", "COMPLEX",
    "CHAR",
    "STR",
    "ELLIPSIS",

    "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULO", "QUOTIENT", "POWER",
    "B_AND", "B_OR", "B_XOR", "LSHIFT", "RSHIFT", "B_NOT",

    "EQUAL", "NEQUAL", "LESS", "GREATER", "LESS_EQ", "GREATER_EQ",
    "AND", "OR", "XOR", "NOT",

    "ASSIGN",
    "INCREMENT", "DECREMENT",
    "PLUS_EQ", "MINUS_EQ", "MULTIPLY_EQ", "DIVIDE_EQ", "MODULO_EQ", "QUOTIENT_EQ", "POWER_EQ",
    "B_AND_EQ", "B_OR_EQ", "B_XOR_EQ", "LSHIFT_EQ", "RSHIFT_EQ",

    "LPAREN", "RPAREN",
    "LSQUARE", "RSQUARE",
    "LCURLY", "RCURLY",

    "QUESTION", "COLON",
    "DOT",
    "COMMA",
    "DOLLAR",

    "PRIVATE_K", "PUBLIC_K",
    "LOCAL_K", "GLOBAL_K",
    "CONST_K", "STATIC_K",

    "VAR_K", "FUNC_K", "CLASS_K", "STRUCT_K",

    "IF_K", "ELIF_K", "ELSE_K",
    "SWITCH_K", "CASE_K", "DEFAULT_K",
    "FOR_K", "TO_K", "STEP_K",
    "DO_K", "WHILE_K",
    "TRY_K", "EXCEPT_K", "FINALLY_K",
    "IMPORT_K", "INCLUDE_K",

    "RETURN_K",
    "CONTINUE_K", "BREAK_K",

    "AND_K", "OR_K", "XOR_K", "NOT_K",
    "IN_K",
    "IS_K", "ARE_K",

    "NONE_K",
    "TRUE_K", "FALSE_K",

    "OBJECT_T",
    "INT_T", "FLOAT_T", "COMPLEX_T",
    "CHAR_T",
    "BOOL_T",
    "STR_T",
    "LIST_T", "TUPLE_T", "DICT_T", "SET_T"
};

token_t token_set1(unsigned char type, char* value, unsigned long long size, pos_p poss, pos_p pose);
token_t token_set2(unsigned char type, pos_p poss, pos_p pose);

void token_print(FILE* stream, token_p token);

void token_p_free1(token_p tokens, unsigned long long size);
void token_p_free2(token_p tokens);

unsigned char identifier_type(const char* identifier);

#endif /* __M_TOKEN__ */
