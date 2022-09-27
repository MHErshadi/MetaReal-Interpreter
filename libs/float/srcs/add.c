/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) + (num2)
/*/

#include <float.h>

void float_add(float_p num1, float_p num2, heap_p heap)
{
    mpfr_add(num1->value, num1->value, num2->value, MPFR_RNDN);
}
