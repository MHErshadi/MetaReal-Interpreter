/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) + (num2)
/*/

#include <float.h>

void float_add_int(float_p num1, int_p num2, heap_p heap)
{
    mpfr_add_z(num1->value, num1->value, num2->value, MPFR_RNDN);
}
