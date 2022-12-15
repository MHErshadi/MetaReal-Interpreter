/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <float.h>

void float_ul_subtract(unsigned long num1, float_p num2)
{
    mpfr_ui_sub(num2->value, num1, num2->value, MPFR_RNDN);
}
