/*/
 * MetaReal version 1.0.0
/*/

#include <lexer/lexer.h>
#include <stdlib.h>
#include <def.h>

#define token_add(type)                                 \
    {                                                   \
        poss = pos;                                     \
                                                        \
        code++;                                         \
        pos.index++;                                    \
                                                        \
        tokens[size++] = token_set2(type, &poss, &pos); \
    }

lres_t lres_fail(illegal_char_p error);
lres_t lres_success(token_p tokens);

const char* skip_comment(const char* code, char terminator, pos_p pos);

const char* gen_identifier(token_p token, const char* code, pos_p pos);
const char* gen_number(token_p token, const char* code, pos_p pos);
const char* gen_char(token_p token, const char* code, char terminator, pos_p pos);
const char* gen_str(token_p token, const char* code, char terminator, pos_p pos);

const char* hand_plus(token_p token, const char* code, pos_p pos);
const char* hand_minus(token_p token, const char* code, pos_p pos);
const char* hand_multiply(token_p token, const char* code, pos_p pos);
const char* hand_divide(token_p token, const char* code, pos_p pos);
const char* hand_modulo(token_p token, const char* code, pos_p pos);
const char* hand_and(token_p token, const char* code, pos_p pos);
const char* hand_or(token_p token, const char* code, pos_p pos);
const char* hand_xor(token_p token, const char* code, pos_p pos);
const char* hand_lshift(token_p token, const char* code, pos_p pos);
const char* hand_rshift(token_p token, const char* code, pos_p pos);
const char* hand_equal(token_p token, const char* code, pos_p pos);
const char* hand_nequal(token_p token, const char* code, pos_p pos);

lres_t lex(const char* code, char terminator)
{
    token_p tokens = malloc(TOKENS_SIZE * sizeof(token_t));

    unsigned long long size = 0;
    unsigned long long alloc = TOKENS_SIZE;

    pos_t pos = pos_set(0, 1);
    pos_t poss;

    while (*code != terminator)
    {
        if (*code == ' ' || *code == '\t')
        {
            code++;
            pos.index++;
            continue;
        }

        if (size == alloc)
            tokens = realloc(tokens, (alloc += TOKENS_SIZE) * sizeof(token_t));

        if (*code == '\n')
        {
            poss = pos;

            code++;
            pos.index++;
            pos.line++;

            if (tokens[size - 1].type == NEWLINE_T)
                continue;

            tokens[size++] = token_set2(NEWLINE_T, &poss, &pos);
            continue;
        }

        if (*code == '#')
        {
            pos.index++;

            code = skip_comment(++code, terminator, &pos);
            continue;
        }

        if ((*code >= 'a' && *code <= 'z') || (*code >= 'A' && *code <= 'Z') || *code == '_')
        {
            code = gen_identifier(&tokens[size++], code, &pos);
            continue;
        }

        if (*code >= '0' && *code <= '9')
        {
            code = gen_number(&tokens[size++], code, &pos);
            continue;
        }

        switch (*code)
        {
        case ';':
            token_add(SEMICOLON_T);
            break;
        case '\'':
            code = gen_char(&tokens[size++], ++code, terminator, &pos);
            break;
        case '"':
            code = gen_str(&tokens[size++], ++code, terminator, &pos);
            break;
        case '.':
            if (code[1] >= '0' && code[1] <= '9')
            {
                code = gen_number(&tokens[size++], code, &pos);
                break;
            }

            poss = pos;

            code++;
            pos.index++;

            if (*code == '.' && code[1] == '.')
            {
                code += 2;
                pos.index += 2;

                tokens[size++] = token_set2(ELLIPSIS_T, &poss, &pos);
                break;
            }

            tokens[size++] = token_set2(DOT_T, &poss, &pos);
            break;
        case '+':
            code = hand_plus(&tokens[size++], ++code, &pos);
            break;
        case '-':
            code = hand_minus(&tokens[size++], ++code, &pos);
            break;
        case '*':
            code = hand_multiply(&tokens[size++], ++code, &pos);
            break;
        case '/':
            code = hand_divide(&tokens[size++], ++code, &pos);
            break;
        case '%':
            code = hand_modulo(&tokens[size++], ++code, &pos);
            break;
        case '&':
            code = hand_and(&tokens[size++], ++code, &pos);
            break;
        case '|':
            code = hand_or(&tokens[size++], ++code, &pos);
            break;
        case '^':
            code = hand_xor(&tokens[size++], ++code, &pos);
            break;
        case '<':
            code = hand_lshift(&tokens[size++], ++code, &pos);
            break;
        case '>':
            code = hand_rshift(&tokens[size++], ++code, &pos);
            break;
        case '~':
            token_add(B_NOT_T);
            break;
        case '=':
            code = hand_equal(&tokens[size++], ++code, &pos);
            break;
        case '!':
            code = hand_nequal(&tokens[size++], ++code, &pos);
            break;
        case '(':
            token_add(LPAREN_T);
            break;
        case ')':
            token_add(RPAREN_T);
            break;
        case '[':
            token_add(LSQUARE_T);
            break;
        case ']':
            token_add(RSQUARE_T);
            break;
        case '{':
            token_add(LCURLY_T);
            break;
        case '}':
            token_add(RCURLY_T);
            break;
        case '?':
            token_add(QUESTION_T);
            break;
        case ':':
            token_add(COLON_T);
            break;
        case ',':
            token_add(COMMA_T);
            break;
        case '$':
            token_add(DOLLAR_T);
            break;
        default:
            token_p_free1(tokens, size);

            illegal_char_t error = illegal_char_set(*code, &pos);
            return lres_fail(&error);
        }
    }

    if (size + 1 != alloc)
        tokens = realloc(tokens, (size + 1) * sizeof(token_t));

    poss = pos;
    pos.index++;

    tokens[size] = token_set2(EOF_T, &poss, &pos);

    return lres_success(tokens);
}

lres_t lres_fail(illegal_char_p error)
{
    lres_t lres;

    lres.error = *error;
    lres.has_error = 1;

    return lres;
}

lres_t lres_success(token_p tokens)
{
    lres_t lres;

    lres.tokens = tokens;
    lres.has_error = 0;

    return lres;
}

const char* skip_comment(const char* code, char terminator, pos_p pos)
{
    if (*code == '*')
    {
        code++;
        pos->index++;

        while (*code != terminator)
        {
            if (*code == '*')
            {
                code++;
                pos->index++;

                if (*code == '#')
                {
                    pos->index++;
                    return ++code;
                }
            }

            if (*code == '\n')
                pos->line++;

            code++;
            pos->index++;
        }

        return code;
    }

    while (*code != '\n' && *code != terminator)
    {
        code++;
        pos->index++;
    }

    if (*code == '\n')
    {
        code++;
        pos->line++;
        pos->index++;
    }

    return code;
}

const char* gen_identifier(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;

    char* identifier = malloc(IDENTIFIER_SIZE);

    unsigned long long size = 0;
    unsigned long long alloc = IDENTIFIER_SIZE;

    do
    {
        if (size == alloc)
            identifier = realloc(identifier, alloc += IDENTIFIER_SIZE);

        identifier[size++] = *code++;
        pos->index++;
    } while ((*code >= 'a' && *code <= 'z') || (*code >= 'A' && *code <= 'Z') || (*code >= '0' && *code <= '9') || *code == '_');

    if (size == alloc)
        identifier = realloc(identifier, ++alloc);
    identifier[size++] = '\0';

    unsigned char type = identifier_type(identifier);
    if (type)
    {
        free(identifier);

        *token = token_set2(type, &poss, pos);
        return code;
    }

    if (size != alloc)
        identifier = realloc(identifier, size);

    *token = token_set1(IDENTIFIER_T, identifier, 0, &poss, pos);
    return code;
}

const char* gen_number(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;

    char* number = malloc(NUMBER_SIZE);

    unsigned long long size = 0;
    unsigned long long alloc = NUMBER_SIZE;

    char is_float = 0;
    do
    {
        if (size == alloc)
            number = realloc(number, alloc += NUMBER_SIZE);

        if (*code == '.')
        {
            if (is_float)
                break;

            is_float = 1;
        }

        number[size++] = *code++;
        pos->index++;
    } while ((*code >= '0' && *code <= '9') || *code == '.');

    if (size + 1 != alloc)
        number = realloc(number, size + 1);
    number[size] = '\0';

    if (*code == 'i')
    {
        pos->index++;

        *token = token_set1(COMPLEX_T, number, size, &poss, pos);
        return ++code;
    }

    if (is_float)
        *token = token_set1(FLOAT_T, number, size, &poss, pos);
    else
        *token = token_set1(INT_T, number, size, &poss, pos);
    return code;
}

const char* gen_char(token_p token, const char* code, char terminator, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    char chr = '\0';
    if (*code != terminator)
    {
        if (*code == '\\')
        {
            pos->index++;
            code++;

            if (*code != terminator)
            {
                switch (*code)
                {
                case '0':
                    chr = '\0';
                    break;
                case 'a':
                    chr = '\a';
                    break;
                case 'b':
                    chr = '\b';
                    break;
                case 'f':
                    chr = '\f';
                    break;
                case 'n':
                    chr = '\n';
                    break;
                case 'r':
                    chr = '\r';
                    break;
                case 't':
                    chr = '\t';
                    break;
                case 'v':
                    chr = '\v';
                    break;
                default:
                    chr = *code;

                    if (*code == '\n')
                        pos->line++;
                    break;
                }

                code++;
                pos->index++;
            }
        }
        else
        {
            chr = *code;

            if (*code++ == '\n')
                pos->line++;
            pos->index++;
        }
    }

    if (*code == '\'')
    {
        code++;
        pos->index++;
    }

    *token = token_set2(CHAR_T, &poss, pos);
    token->size = chr;
    return code;
}

const char* gen_str(token_p token, const char* code, char terminator, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    char* str = malloc(STRING_SIZE);

    unsigned long long size = 0;
    unsigned long long alloc = STRING_SIZE;

    char escape = 0;
    while ((*code != '"' || escape) && *code != terminator)
    {
        if (size == alloc)
            str = realloc(str, alloc += STRING_SIZE);

        if (escape)
        {
            switch (*code)
            {
            case '0':
                str[size++] = '\0';
                break;
            case 'a':
                str[size++] = '\a';
                break;
            case 'b':
                str[size++] = '\b';
                break;
            case 'f':
                str[size++] = '\f';
                break;
            case 'n':
                str[size++] = '\n';
                break;
            case 'r':
                str[size++] = '\r';
                break;
            case 't':
                str[size++] = '\t';
                break;
            case 'v':
                str[size++] = '\v';
                break;
            default:
                str[size++] = *code;

                if (*code == '\n')
                    pos->line++;
                break;
            }

            code++;
            pos->index++;

            escape = 0;
            continue;
        }

        if (*code == '\\')
        {
            escape = 1;

            code++;
            pos->index++;
            continue;
        }

        if (*code == '\n')
            pos->line++;

        str[size++] = *code++;
        pos->index++;
    }

    if (*code != terminator)
    {
        code++;
        pos->index++;
    }

    if (size + 1 != alloc)
        str = realloc(str, size + 1);
    str[size] = '\0';

    *token = token_set1(STR_T, str, size, &poss, pos);
    return code;
}

const char* hand_plus(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(PLUS_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '+')
    {
        pos->index++;

        *token = token_set2(INCREMENT_T, &poss, pos);
        return ++code;
    }

    *token = token_set2(PLUS_T, &poss, pos);
    return code;
}

const char* hand_minus(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(MINUS_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '-')
    {
        pos->index++;

        *token = token_set2(DECREMENT_T, &poss, pos);
        return ++code;
    }

    *token = token_set2(MINUS_T, &poss, pos);
    return code;
}

const char* hand_multiply(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(MULTIPLY_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '*')
    {
        pos->index++;

        if (*++code == '=')
        {
            pos->index++;

            *token = token_set2(POWER_EQ_T, &poss, pos);
            return ++code;
        }

        *token = token_set2(POWER_T, &poss, pos);
        return code;
    }

    *token = token_set2(MULTIPLY_T, &poss, pos);
    return code;
}

const char* hand_divide(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(DIVIDE_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '/')
    {
        pos->index++;

        if (*++code == '=')
        {
            pos->index++;

            *token = token_set2(QUOTIENT_EQ_T, &poss, pos);
            return ++code;
        }

        *token = token_set2(QUOTIENT_T, &poss, pos);
        return code;
    }

    *token = token_set2(DIVIDE_T, &poss, pos);
    return code;
}

const char* hand_modulo(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(MODULO_EQ_T, &poss, pos);
        return ++code;
    }

    *token = token_set2(MODULO_T, &poss, pos);
    return code;
}

const char* hand_and(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(B_AND_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '&')
    {
        pos->index++;

        *token = token_set2(AND_T, &poss, pos);
        return ++code;
    }

    *token = token_set2(B_AND_T, &poss, pos);
    return code;
}

const char* hand_or(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(B_OR_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '|')
    {
        pos->index++;

        *token = token_set2(OR_T, &poss, pos);
        return ++code;
    }

    *token = token_set2(B_OR_T, &poss, pos);
    return code;
}

const char* hand_xor(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(B_XOR_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '^')
    {
        pos->index++;

        *token = token_set2(XOR_T, &poss, pos);
        return ++code;
    }

    *token = token_set2(B_XOR_T, &poss, pos);
    return code;
}

const char* hand_lshift(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(LESS_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '<')
    {
        pos->index++;

        if (*++code == '=')
        {
            pos->index++;

            *token = token_set2(LSHIFT_EQ_T, &poss, pos);
            return ++code;
        }

        *token = token_set2(LSHIFT_T, &poss, pos);
        return code;
    }

    *token = token_set2(LESS_T, &poss, pos);
    return code;
}

const char* hand_rshift(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        *token = token_set2(GREATER_EQ_T, &poss, pos);
        return ++code;
    }
    if (*code == '>')
    {
        pos->index++;

        if (*++code == '=')
        {
            pos->index++;

            *token = token_set2(RSHIFT_EQ_T, &poss, pos);
            return ++code;
        }

        *token = token_set2(RSHIFT_T, &poss, pos);
        return code;
    }

    *token = token_set2(GREATER_T, &poss, pos);
    return code;
}

const char* hand_equal(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        if (*++code == '=')
        {
            pos->index++;

            *token = token_set2(EX_EQUAL_T, &poss, pos);
            return ++code;
        }

        *token = token_set2(EQUAL_T, &poss, pos);
        return code;
    }

    *token = token_set2(ASSIGN_T, &poss, pos);
    return code;
}

const char* hand_nequal(token_p token, const char* code, pos_p pos)
{
    pos_t poss = *pos;
    pos->index++;

    if (*code == '=')
    {
        pos->index++;

        if (*++code == '=')
        {
            pos->index++;

            *token = token_set2(EX_NEQUAL_T, &poss, pos);
            return ++code;
        }

        *token = token_set2(NEQUAL_T, &poss, pos);
        return code;
    }

    *token = token_set2(NOT_T, &poss, pos);
    return code;
}
