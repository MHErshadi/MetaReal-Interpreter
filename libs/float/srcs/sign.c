/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Returns sign of (num)
 * 1 if positive, 0 if zero and -1 if negative
/*/

#include <float.h>

char float_sign(float_p num)
{
    return mpfr_sgn(num->value);
}
