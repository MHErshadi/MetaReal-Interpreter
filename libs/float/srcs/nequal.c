/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) != (num2)
/*/

#include <float.h>

char float_nequal(const float_p num1, const float_p num2)
{
    return !mpfr_equal_p(num1->value, num2->value);
}
