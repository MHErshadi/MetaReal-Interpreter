/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Converts (str) into float based on (size)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_set_str(unsigned long long prec, const char* str, unsigned long long size)
{
    float_p dst = malloc(sizeof(float_t));

    mpfr_init2(dst->value, prec);
    mpfr_set_str(dst->value, str, 10, MPFR_RNDN);

    return dst;
}
