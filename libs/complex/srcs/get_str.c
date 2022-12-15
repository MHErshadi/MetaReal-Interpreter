/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Converts (src) into str
/*/

#include <complex.h>
#include <string.h>
#include <stdlib.h>

char* complex_get_str(const complex_p src, unsigned long long prec)
{
    unsigned long long prec_c = prec;
    unsigned char length = 0;

    do
    {
        prec_c /= 10;
        length++;
    } while (prec_c);

    if (mpfr_sgn(mpc_realref(src->value)))
    {
        char* format = malloc(13 + length * 2);
        sprintf(format, "(%%.%lluRg%%+.%lluRgi)", prec, prec);

        char* str = malloc(prec * 2 + 100);
        mpfr_sprintf(str, format, mpc_realref(src->value), mpc_imagref(src->value));

        str = realloc(str, strlen(str) + 1);

        free(format);
        return str;
    }
    else
    {
        char* format = malloc(6 + length);
        sprintf(format, "%%.%lluRgi", prec);

        char* str = malloc(prec + 100);
        mpfr_sprintf(str, format, mpc_imagref(src->value));

        str = realloc(str, strlen(str) + 1);

        free(format);
        return str;
    }
}
