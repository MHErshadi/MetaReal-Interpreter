/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_subtract_int(const float_p num1, const int_p num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, float_prec_bit);

    mpfr_sub_z(res->value, num1->value, num2->value, MPFR_RNDN);

    return res;
}

void float_subtract_int_self(float_p num1, const int_p num2)
{
    mpfr_sub_z(num1->value, num1->value, num2->value, MPFR_RNDN);
}
