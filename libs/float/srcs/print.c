/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Prints (num) + (end) into (stream)
/*/

#include <float.h>
#include <stdlib.h>

unsigned char float_number_length(unsigned long long number);

void float_print(FILE* stream, float_p num, unsigned long long prec, const char* end)
{
    char* format = malloc(7 + float_number_length(prec));
    sprintf(format, "%%.%lluRg%%s", prec);

    mpfr_fprintf(stream, format, num->value, end);

    free(format);
}

unsigned char float_number_length(unsigned long long number)
{
    unsigned char length = 0;

    do
    {
        number /= 10;
        length++;
    } while (number);

    return length;
}
