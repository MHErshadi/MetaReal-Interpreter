/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) < (num2)
/*/

#include <float.h>

char float_int_less(int_p num1, float_p num2)
{
    return mpfr_cmp_z(num2->value, num1->value) == -1;
}
