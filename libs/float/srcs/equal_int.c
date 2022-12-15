/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) == (num2)
/*/

#include <float.h>

char float_equal_int(const float_p num1, const int_p num2)
{
    return !mpfr_cmp_z(num1->value, num2->value);
}
