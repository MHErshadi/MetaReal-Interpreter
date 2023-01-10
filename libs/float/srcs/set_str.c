/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Converts (str) into float based on (size)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_set_str(const char* str, unsigned long long size, unsigned char base)
{
    float_p dst = malloc(sizeof(float_t));

    mpfr_init2(dst->value, float_prec_bit);
    mpfr_set_str(dst->value, str, base, MPFR_RNDN);

    dst->ref = 0;

    return dst;
}
