/*/
 * MetaReal version 1.0.0
/*/

#include <debugger/errlib.h>
#include <setting.h>
#include <stdlib.h>
#include <string.h>
#include <interpreter/value.h>

unsigned char number_length(unsigned long long number);

const char* runtime_labels[8] =
{
    "TypeError",
    "IllegalOpError",
    "MemOverflowError",
    "OutRangeError",
    "DivByZeroError",
    "NotDefinedError",
    "ConstError",
    "AccessError"
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

runtime_t runtime_set(unsigned char type, char* detail, pos_p poss, pos_p pose, context_p context)
{
    runtime_t error;

    error.type = type;
    error.detail = detail;
    error.poss = *poss;
    error.pose = *pose;
    error.context = context;

    return error;
}

void runtime_print(runtime_p error, const char* code, unsigned long long size)
{
    fprintf(setting.error, "\nRuntime Error: %s\n", error->detail);
    free(error->detail);

    fprintf(setting.error, "Error Type: %s (#id=%u)\n", runtime_labels[error->type], error->type);

    context_p context = error->context;
    pos_p pos = &error->poss;

    unsigned long long length = strlen(context->fname) + strlen(context->name) + number_length(pos->line) + 23;

    char* troubleshoot = malloc(length);
    sprintf(troubleshoot, "  File \"%s\", line %llu, in %s\n", context->fname, pos->line, context->name);

    while (context->parent)
    {
        pos = &context->parent_pos;
        context = context->parent;

        length += strlen(context->fname) + strlen(context->name) + number_length(pos->line) + 23;

        char* tail = malloc(length);
        sprintf(tail, "  File \"%s\", line %llu, in %s\n%s", context->fname, pos->line + 1, context->name, troubleshoot);

        free(troubleshoot);
        troubleshoot = tail;
    }

    fprintf(setting.error, "\nTroubleshoot (most recent call last):\n%s\n", troubleshoot);
    free(troubleshoot);

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

unsigned char number_length(unsigned long long number)
{
    unsigned char length = 0;

    do
    {
        number /= 10;
        length++;
    } while (number);

    return length;
}
