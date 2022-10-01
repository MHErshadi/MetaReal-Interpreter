/*/
 * MetaReal version 1.0.0
/*/

#include <lexer/token.h>
#include <stdio.h>
#include <string.h>

#define KEYWORDS_PAD PRIVATE_TK
#define KEYWORDS_COUNT 40

#define TYPES_PAD OBJECT_TT
#define TYPES_COUNT 11

const char* token_labels[110] =
{
    "EOF", "NEWLINE", "SEMICOLON",

    "IDENTIFIER",

    "INT", "FLOAT", "COMPLEX",
    "CHAR",
    "STR",
    "ELLIPSIS",

    "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULO", "QUOTIENT", "POWER",
    "B_AND", "B_OR", "B_XOR", "B_LSHIFT", "B_RSHIFT", "B_NOT",

    "EQUAL", "NEQUAL", "LESS", "GREATER", "LESS_EQ", "GREATER_EQ",
    "AND", "OR", "XOR", "NOT",

    "ASSIGN",
    "INCREMENT", "DECREMENT",
    "PLUS_EQ", "MINUS_EQ", "MULTIPLY_EQ", "DIVIDE_EQ", "MODULO_EQ", "QUOTIENT_EQ", "POWER_EQ",
    "B_AND_EQ", "B_OR_EQ", "B_XOR_EQ", "B_LSHIFT_EQ", "B_RSHIFT_EQ",

    "LPAREN", "RPAREN",
    "LSQUARE", "RSQUARE",
    "LCURLY", "RCURLY",

    "QUESTION", "COLON",
    "DOT",
    "COMMA",
    "DOLLAR",

    "private", "public",
    "local", "global",
    "const", "static",

    "var", "func", "class", "struct", "enum",

    "if", "elif", "else",
    "switch", "case", "default",
    "for", "to", "step",
    "do", "while",
    "try", "except", "finally",
    "import", "include",

    "return",
    "continue", "break",

    "and", "or", "xor", "not",
    "in",
    "is", "are",

    "none",
    "true", "false",

    "object",
    "int", "float", "complex",
    "char",
    "bool",
    "str",
    "list", "tuple", "dict", "set"
};

const char* keywords[KEYWORDS_COUNT] =
{
    "private", "public",
    "local", "global",
    "const", "static",

    "var", "func", "class", "struct", "enum",

    "if", "elif", "else",
    "switch", "case", "default",
    "for", "to", "step",
    "do", "while",
    "try", "except", "finally",
    "import", "include",

    "return",
    "continue", "break",

    "and", "or", "xor", "not",
    "in",
    "is", "are",

    "none",
    "true", "false"
};

const char* types[TYPES_COUNT] =
{
    "object",
    "int", "float", "complex",
    "char",
    "bool",
    "str",
    "list", "tuple", "dict", "set"
};

token_t token_set1(unsigned char type, const char* value, unsigned long long size, pos_t poss, pos_t pose)
{
    token_t token;

    token.type = type;
    token.value = value;
    token.size = size;
    token.poss = poss;
    token.pose = pose;

    return token;
}

token_t token_set2(unsigned char type, pos_t poss, pos_t pose)
{
    token_t token;

    token.type = type;
    token.poss = poss;
    token.pose = pose;

    return token;
}

void token_print(token_p token)
{
    printf(token_labels[token->type]);
}

unsigned char identifier_type(const char* identifier)
{
    unsigned char i;
    for (i = 0; i < KEYWORDS_COUNT; i++)
        if (!strcmp(identifier, keywords[i]))
            return i + KEYWORDS_PAD;

    for (i = 0; i < TYPES_COUNT; i++)
        if (!strcmp(identifier, types[i]))
            return i + TYPES_PAD;

    return 0;
}
