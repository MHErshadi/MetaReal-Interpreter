/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <float.h>

void float_int_subtract(int_p num1, float_p num2)
{
    mpfr_z_sub(num2->value, num1->value, num2->value, MPFR_RNDN);
}
