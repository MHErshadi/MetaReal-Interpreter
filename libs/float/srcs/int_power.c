/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_int_power(const int_p num1, const float_p num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, float_prec_bit);
    mpfr_set_z(res->value, num1->value, MPFR_RNDN);

    mpfr_pow(res->value, res->value, num2->value, MPFR_RNDN);

    return res;
}

void float_int_power_self(const int_p num1, float_p num2)
{
    mpfr_t num1f;
    mpfr_init2(num1f, float_prec_bit);
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpfr_pow(num2->value, num1f, num2->value, MPFR_RNDN);

    mpfr_clear(num1f);
}
