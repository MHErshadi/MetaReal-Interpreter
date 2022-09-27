/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>

int_p float_quotient(float_p num1, float_p num2, cellular_p cellular, heap_p heap)
{
    mpfr_div(num1->value, num1->value, num2->value, MPFR_RNDN);

    int_p res = cellular_alloc(cellular);

    mpz_init(res->value);
    mpfr_get_z(res->value, num1->value, MPFR_RNDD);

    return res;
}
