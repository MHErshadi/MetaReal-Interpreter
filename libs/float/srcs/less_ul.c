/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) < (num2)
/*/

#include <float.h>

char float_less_ul(const float_p num1, unsigned long num2)
{
    return mpfr_cmp_ui(num1->value, num2) < 0;
}
