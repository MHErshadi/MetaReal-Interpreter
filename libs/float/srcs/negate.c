/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * -(num)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_negate(const float_p num)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, float_prec_bit);

    mpfr_neg(res->value, num->value, MPFR_RNDN);

    return res;
}

void float_negate_self(float_p num)
{
    mpfr_neg(num->value, num->value, MPFR_RNDN);
}
