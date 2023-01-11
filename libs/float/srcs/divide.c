/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_divide(const float_p num1, const float_p num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, float_prec_bit);

    mpfr_div(res->value, num1->value, num2->value, MPFR_RNDN);

    return res;
}

void float_divide_self(float_p num1, const float_p num2)
{
    mpfr_div(num1->value, num1->value, num2->value, MPFR_RNDN);
}
