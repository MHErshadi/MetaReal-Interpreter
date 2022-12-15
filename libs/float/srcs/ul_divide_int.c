/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <float.h>
#include <stdlib.h>

float_p float_ul_divide_int(unsigned long num1, const int_p num2, unsigned long long prec)
{
    float_p res = malloc(sizeof(float_t));

    mpfr_init2(res->value, prec);
    mpfr_set_ui(res->value, num1, MPFR_RNDN);

    mpfr_div_z(res->value, res->value, num2->value, MPFR_RNDN);

    return res;
}