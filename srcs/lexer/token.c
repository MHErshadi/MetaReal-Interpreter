/*/
 * MetaReal version 1.0.0
/*/

#include <lexer/token.h>
#include <string.h>

#define KEYWORDS_PAD PRIVATE_TK
#define KEYWORDS_COUNT 40

#define TYPES_PAD OBJECT_TT
#define TYPES_COUNT 11

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
        fputs("(CHAR: '", stream);

        switch (token->size)
        {
        case '\0':
            fputs("\\0')", stream);
            return;
        case '\a':
            fputs("\\a')", stream);
            return;
        case '\b':
            fputs("\\b')", stream);
            return;
        case '\f':
            fputs("\\f')", stream);
            return;
        case '\n':
            fputs("\\n')", stream);
            return;
        case '\r':
            fputs("\\r')", stream);
            return;
        case '\t':
            fputs("\\t')", stream);
            return;
        case '\v':
            fputs("\\v')", stream);
            return;
        default:
            fprintf(stream, "%c')", token->size);
            return;
        }
    }

    if (token->type == STR_T)
    {
        fputs("(STR: \"", stream);

        unsigned long long i;
        for (i = 0; i < token->size; i++)
            switch (token->value[i])
            {
            case '\0':
                fputs("\\0", stream);
                break;
            case '\a':
                fputs("\\a", stream);
                break;
            case '\b':
                fputs("\\b", stream);
                break;
            case '\f':
                fputs("\\f", stream);
                break;
            case '\n':
                fputs("\\n", stream);
                break;
            case '\r':
                fputs("\\r", stream);
                break;
            case '\t':
                fputs("\\t", stream);
                break;
            case '\v':
                fputs("\\v", stream);
                break;
            default:
                putc(token->value[i], stream);
                break;
            }

        fputs("\")", stream);
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
