/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Prints (str) + (end) into (stream)
/*/

#include <str.h>

void str_print(FILE* stream, const str_p str, const char* end)
{
    fprintf(stream, "%s%s", str->str, end);
}
