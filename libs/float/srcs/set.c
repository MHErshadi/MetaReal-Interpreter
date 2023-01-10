/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Copies (src)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_set(const float_p src)
{
    float_p dst = malloc(sizeof(float_t));

    mpfr_init2(dst->value, prec_bit);
    mpfr_set(dst->value, src->value, MPFR_RNDN);

    dst->ref = 0;

    return dst;
}
