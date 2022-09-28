/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Prints (num) + (end) into (stream)
/*/

#include <float.h>

void float_print(FILE* stream, float_p num, unsigned long long prec, const char* end)
{
    mpfr_fprintf(stream, "%.*Rg%s", prec, num->value, end);
}
