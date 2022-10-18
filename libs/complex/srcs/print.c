/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Prints (num) + (end) into (stream)
/*/

#include <complex.h>
#include <stdlib.h>

unsigned char complex_number_length(unsigned long long number);

void complex_print(FILE* stream, complex_p num, unsigned long long prec, const char* end)
{
    if (mpfr_sgn(mpc_realref(num->value)))
    {
        char* format = malloc(14 + complex_number_length(prec) * 2);
        sprintf(format, "(%%.%lluRg+.%lluRgi)%%s", prec, prec);

        mpfr_fprintf(stream, format, mpc_realref(num->value), mpc_imagref(num->value), end);

        free(format);
    }
    else
    {
        char* format = malloc(8 + complex_number_length(prec));
        sprintf(format, "%%.%lluRgi%%s", prec);

        mpfr_fprintf(stream, format, mpc_imagref(num->value), end);

        free(format);
    }
}

unsigned char complex_number_length(unsigned long long number)
{
    unsigned char length = 0;

    do
    {
        number /= 10;
        length++;
    } while (number);

    return length;
}
