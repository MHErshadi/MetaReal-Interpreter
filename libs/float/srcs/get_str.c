/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Converts (src) into str
/*/

#include <float.h>
#include <string.h>
#include <stdlib.h>

char* float_get_str(const float_p src)
{
    unsigned long long prec_c = prec_show;
    unsigned char length = 0;

    do
    {
        prec_c /= 10;
        length++;
    } while (prec_c);

    char* format = malloc(5 + length);
    sprintf(format, "%%.%lluRg", prec_show);

    char* str = malloc(prec_show + 100);
    mpfr_sprintf(str, format, src->value);

    str = realloc(str, strlen(str) + 1);

    free(format);
    return str;
}
