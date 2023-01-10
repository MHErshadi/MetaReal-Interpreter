/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_ul_modulo(unsigned long num1, const float_p num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, prec_bit);
    mpfr_set_ui(res->value, num1, MPFR_RNDN);

    res->ref = 0;

    mpfr_remainder(res->value, res->value, num2->value, MPFR_RNDN);

    return res;
}
