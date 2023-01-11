/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>
#include <stdlib.h>

int_p float_int_quotient(const int_p num1, const float_p num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);

    mpfr_t num1f;
    mpfr_init2(num1f, float_prec_bit);
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpfr_div(num1f, num1f, num2->value, MPFR_RNDN);

    mpfr_get_z(res->value, num1f, MPFR_RNDD);

    mpfr_clear(num1f);
    return res;
}

void float_int_quotient_self(int_p num1, const float_p num2)
{
    mpfr_t num1f;
    mpfr_init2(num1f, float_prec_bit);
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpfr_div(num1f, num1f, num2->value, MPFR_RNDN);

    mpfr_get_z(num1->value, num1f, MPFR_RNDD);

    mpfr_clear(num1f);
}
