/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_modulo_ul(const float_p num1, unsigned long num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, float_prec_bit);
    mpfr_set_ui(res->value, num2, MPFR_RNDN);

    mpfr_remainder(res->value, num1->value, res->value, MPFR_RNDN);

    return res;
}

void float_modulo_ul_self(float_p num1, unsigned long num2)
{
    mpfr_t num2f;
    mpfr_init2(num2f, float_prec_bit);
    mpfr_set_ui(num2f, num2, MPFR_RNDN);

    mpfr_remainder(num1->value, num1->value, num2f, MPFR_RNDN);

    mpfr_clear(num2f);
}
