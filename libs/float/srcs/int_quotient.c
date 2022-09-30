/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>

void float_int_quotient(int_p num1, float_p num2)
{
    mpfr_t num1f;
    mpfr_init2(num1f, mpfr_get_prec(num2->value));
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpfr_div(num1f, num1f, num2->value, MPFR_RNDN);

    mpz_clear(num1->value);

    mpz_init(num1->value);
    mpfr_get_z(num1->value, num1f, MPFR_RNDD);

    mpfr_clear(num1f);
}
