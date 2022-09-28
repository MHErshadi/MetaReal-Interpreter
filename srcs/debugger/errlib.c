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

void illegal_char_print(illegal_char_p error, const char* code, unsigned long long size, const char* fname)
{
    fprintf(setting.error, "\nIllegal Character Error: '%c'\n", error->chr);
    fprintf(setting.error, "File \"%s\", line %llu\n\n", fname, error->pos.line);
}

void invalid_syntax_print(invalid_syntax_p error, const char* code, unsigned long long size, const char* fname)
{
    if (error->detail)
        fprintf(setting.error, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", setting.error);
    fprintf(setting.error, "File \"%s\", line %llu\n\n", fname, error->poss.line);
}

void runtime_print(runtime_p error, const char* code, unsigned long long size)
{
    fprintf(setting.error, "\nRuntime Error: %s\n", error->detail);
    free(error->detail);

    fprintf(setting.error, "Error Type: %s (#id=%u)\n\n", runtime_labels[error->type], error->type);
}
