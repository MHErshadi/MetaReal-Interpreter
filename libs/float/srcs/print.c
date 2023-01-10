/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Prints (num) + (end) into (stream)
/*/

#include <float.h>
#include <stdlib.h>

void float_print(FILE* stream, const float_p num, const char* end)
{
    unsigned long long prec_c = float_prec_show;
    unsigned char length = 0;

    do
    {
        prec_c /= 10;
        length++;
    } while (prec_c);

    char* format = malloc(7 + length);
    sprintf(format, "%%.%lluRg%%s", float_prec_show);

    mpfr_fprintf(stream, format, num->value, end);

    free(format);
}
