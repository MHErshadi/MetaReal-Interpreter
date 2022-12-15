/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>
#include <stdlib.h>

int_p float_quotient_ul(float_p num1, unsigned long num2)
{
    mpfr_div_ui(num1->value, num1->value, num2, MPFR_RNDN);

    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);
    mpfr_get_z(res->value, num1->value, MPFR_RNDD);

    return res;
}
