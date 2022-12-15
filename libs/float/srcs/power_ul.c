/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <float.h>

void float_power_ul(float_p num1, unsigned long num2)
{
    mpfr_pow_ui(num1->value, num1->value, num2, MPFR_RNDN);
}