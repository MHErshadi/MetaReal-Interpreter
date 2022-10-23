/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Prints (str) + (end) into (stream) (in label form)
/*/

#include <str.h>

void str_label(FILE* stream, str_p str, const char* end)
{
    fputc('"', stream);

    unsigned long long i;
    for (i = 0; i < str->size; i++)
        switch (str->str[i])
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
            fputc(str->str[i], stream);
            break;
        }

    fprintf(stream, "\"%s", end);
}
