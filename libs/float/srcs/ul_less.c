/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) < (num2)
/*/

#include <float.h>

char float_ul_less(unsigned long num1, const float_p num2)
{
    return mpfr_cmp_ui(num2->value, num1) > 0;
}
