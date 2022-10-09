/*/
 * MetaReal version 1.0.0
/*/

#include <debugger/errlib.h>
#include <setting.h>
#include <stdlib.h>

const char* runtime_labels[1] =
{
    "DivByZero"
};

illegal_char_t illegal_char_set(char chr, pos_p pos)
{
    illegal_char_t error;

    error.chr = chr;
    error.pos = *pos;

    return error;
}

void illegal_char_print(illegal_char_p error, const char* code, unsigned long long size, const char* fname)
{
    fprintf(setting.error, "\nIllegal Character Error: '%c'\n", error->chr);
    fprintf(setting.error, "File \"%s\", line %llu\n\n", fname, error->pos.line);

    unsigned long long start;
    for (start = error->pos.index; code[start] != '\n' && start > 0; start--);
    if (start)
        start++;

    unsigned long long end;
    for (end = error->pos.index; code[end] != '\n' && end < size; end++);

    unsigned long long i;
    for (i = start; i < end; i++)
        putc(code[i], setting.error);
    putc('\n', setting.error);

    for (i = start; i < error->pos.index; i++)
        putc(' ', setting.error);
    fputs("^\n\n", setting.error);
}

invalid_syntax_t invalid_syntax_set(const char* detail, pos_p poss, pos_p pose)
{
    invalid_syntax_t error;

    error.detail = detail;
    error.poss = *poss;
    error.pose = *pose;

    return error;
}

void invalid_syntax_print(invalid_syntax_p error, const char* code, unsigned long long size, const char* fname)
{
    if (error->detail)
        fprintf(setting.error, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", setting.error);
    fprintf(setting.error, "File \"%s\", line %llu\n\n", fname, error->poss.line);

    if (error->poss.line != error->pose.line)
    {
        unsigned long long start;
        for (start = error->poss.index; code[start] != '\n' && start > 0; start--);
        if (start)
            start++;

        unsigned long long end;
        for (end = error->poss.index; code[end] != '\n' && end < size; end++);

        unsigned long long i;
        for (i = start; i < end; i++)
            putc(code[i], setting.error);
        putc('\n', setting.error);

        for (i = start; i < error->poss.index; i++)
            putc(' ', setting.error);
        for (; i < end; i++)
            putc('^', setting.error);
        fputs("~\n\n", setting.error);

        return;
    }

    unsigned long long start;
    for (start = error->poss.index; code[start] != '\n' && start > 0; start--);
    if (start)
        start++;

    unsigned long long end;
    for (end = error->pose.index; code[end] != '\n' && end < size; end++);

    unsigned long long i;
    for (i = start; i < end; i++)
        putc(code[i], setting.error);
    putc('\n', setting.error);

    for (i = start; i < error->poss.index; i++)
        putc(' ', setting.error);
    for (; i < error->pose.index; i++)
        putc('^', setting.error);
    fputs("\n\n", setting.error);
}

runtime_t runtime_set(unsigned char type, char* detail, pos_p poss, pos_p pose)
{
    runtime_t error;

    error.type = type;
    error.detail = detail;
    error.poss = *poss;
    error.pose = *pose;

    return error;
}

void runtime_print(runtime_p error, const char* code, unsigned long long size)
{
    fprintf(setting.error, "\nRuntime Error: %s\n", error->detail);
    free(error->detail);

    fprintf(setting.error, "Error Type: %s (#id=%u)\n\n", runtime_labels[error->type], error->type);
}
