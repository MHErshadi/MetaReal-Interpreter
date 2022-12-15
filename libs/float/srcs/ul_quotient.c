/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>
#include <stdlib.h>

int_p float_ul_quotient(unsigned long num1, float_p num2)
{
    mpfr_ui_div(num2->value, num1, num2->value, MPFR_RNDN);

    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);
    mpfr_get_z(res->value, num2->value, MPFR_RNDD);

    return res;
}
