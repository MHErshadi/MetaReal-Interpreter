/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_ul_divide_ul(unsigned long num1, unsigned long num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, prec_bit);
    mpfr_set_ui(res->value, num1, MPFR_RNDN);

    res->ref = 0;

    mpfr_div_ui(res->value, res->value, num2, MPFR_RNDN);

    return res;
}
