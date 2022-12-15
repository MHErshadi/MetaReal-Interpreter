/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <float.h>

void float_divide_int(float_p num1, const int_p num2)
{
    mpfr_div_z(num1->value, num1->value, num2->value, MPFR_RNDN);
}
