/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) < (num2)
/*/

#include <float.h>

char float_less(float_p num1, float_p num2)
{
    return mpfr_less_p(num1->value, num2->value);
}
