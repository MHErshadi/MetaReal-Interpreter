/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) < (num2)
/*/

#include <float.h>

char float_int_less(const int_p num1, const float_p num2)
{
    return mpfr_cmp_z(num2->value, num1->value) > 0;
}
