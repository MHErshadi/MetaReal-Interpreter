/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * -(num)
/*/

#include <float.h>

void float_negate(float_p num)
{
    mpfr_neg(num->value, num->value, MPFR_RNDN);
}
