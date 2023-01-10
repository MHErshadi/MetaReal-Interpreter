/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Returns (1) if (num) was zero and (0) otherwise
/*/

#include <complex.h>

char complex_is_zero(const complex_p num)
{
    return !mpfr_sgn(mpc_realref(num->value)) && !mpfr_sgn(mpc_imagref(num->value));
}
