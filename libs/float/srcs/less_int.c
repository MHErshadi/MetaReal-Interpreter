/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) < (num2)
/*/

#include <float.h>

char float_less_int(float_p num1, int_p num2)
{
    return mpfr_cmp_z(num1->value, num2->value) == 1;
}
