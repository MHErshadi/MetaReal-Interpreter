/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <float.h>
#include <memory.h>

float_p float_int_divide_int(int_p num1, int_p num2, unsigned long long prec)
{
    float_p res = cellular_alloc(&memory.float_cellular);

    mpfr_init2(res->value, prec);
    mpfr_set_z(res->value, num1->value, MPFR_RNDN);

    mpfr_div_z(res->value, res->value, num2->value, MPFR_RNDN);

    return res;
}
