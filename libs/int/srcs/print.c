/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Prints (num) + (end) into (stream)
/*/

#include <int.h>

void int_print(FILE* stream, const int_p num, const char* end)
{
    gmp_fprintf(stream, "%Zd%s", num->value, end);
}
