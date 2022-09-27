/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>

void float_quotient_int(float_p num1, int_p num2, heap_p heap)
{
    mpfr_div_z(num1->value, num1->value, num2->value, MPFR_RNDN);

    mpz_clear(num2->value);

    mpz_init(num2->value);
    mpfr_get_z(num2->value, num1->value, MPFR_RNDD);
}
