/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_divide_ul(const float_p num1, unsigned long num2)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, prec_bit);
    res->ref = 0;

    mpfr_div_ui(res->value, num1->value, num2, MPFR_RNDN);

    return res;
}
