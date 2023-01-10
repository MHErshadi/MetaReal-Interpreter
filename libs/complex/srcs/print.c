/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Prints (num) + (end) into (stream)
/*/

#include <complex.h>
#include <stdlib.h>

void complex_print(FILE* stream, const complex_p num, const char* end)
{
    unsigned long long prec_c = complex_prec_show;
    unsigned char length = 0;

    do
    {
        prec_c /= 10;
        length++;
    } while (prec_c);

    if (mpfr_sgn(mpc_realref(num->value)))
    {
        char* format = malloc(15 + length * 2);
        sprintf(format, "(%%.%lluRg%%+.%lluRgi)%%s", complex_prec_show, complex_prec_show);

        mpfr_fprintf(stream, format, mpc_realref(num->value), mpc_imagref(num->value), end);

        free(format);
    }
    else
    {
        char* format = malloc(8 + length);
        sprintf(format, "%%.%lluRgi%%s", complex_prec_show);

        mpfr_fprintf(stream, format, mpc_imagref(num->value), end);

        free(format);
    }
}
