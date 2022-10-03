/*/
 * MetaReal version 1.0.0
/*/

#include <lexer/token.h>
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

    "VAR_K", "FUNC_K", "CLASS_K", "STRUCT_K", "ENUM_K",

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

token_t token_set1(unsigned char type, const char* value, unsigned long long size, pos_p poss, pos_p pose)
{
    token_t token;

    token.type = type;
    token.value = value;
    token.size = size;
    token.poss = *poss;
    token.pose = *pose;

    return token;
}

token_t token_set2(unsigned char type, pos_p poss, pos_p pose)
{
    token_t token;

    token.type = type;
    token.poss = *poss;
    token.pose = *pose;

    return token;
}

void token_print(FILE* stream, token_p token)
{
    if (token->type >= IDENTIFIER_T && token->type <= COMPLEX_T)
    {
        fprintf(stream, "(%s: %s)", token_labels[token->type], token->value);
        return;
    }

    if (token->type == CHAR_T)
    {
        fprintf(stream, "(%s: '", token_labels[token->type]);

        switch (token->size)
        {
        case '\0':
            fprintf(stream, "\\0')");
            return;
        case '\a':
            fprintf(stream, "\\a')");
            return;
        case '\b':
            fprintf(stream, "\\b')");
            return;
        case '\f':
            fprintf(stream, "\\f')");
            return;
        case '\n':
            fprintf(stream, "\\n')");
            return;
        case '\r':
            fprintf(stream, "\\r')");
            return;
        case '\t':
            fprintf(stream, "\\t')");
            return;
        case '\v':
            fprintf(stream, "\\v')");
            return;
        default:
            fprintf(stream, "%c')", token->size);
            return;
        }
    }

    if (token->type == STR_T)
    {
        fprintf(stream, "(%s: \"", token_labels[token->type]);

        unsigned long long i = 0;
        while (token->value[i])
        {
            switch (token->value[i])
            {
            case '\a':
                fprintf(stream, "\\a");
                break;
            case '\b':
                fprintf(stream, "\\b");
                break;
            case '\f':
                fprintf(stream, "\\f");
                break;
            case '\n':
                fprintf(stream, "\\n");
                break;
            case '\r':
                fprintf(stream, "\\r");
                break;
            case '\t':
                fprintf(stream, "\\t");
                break;
            case '\v':
                fprintf(stream, "\\v");
                break;
            default:
                putc(token->value[i], stream);
                break;
            }

            i++;
        }

        fprintf(stream, "\")");
        return;
    }

    fprintf(stream, "(%s)", token_labels[token->type]);
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
