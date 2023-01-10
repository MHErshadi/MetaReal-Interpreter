/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) // (num2)
/*/

#include <float.h>
#include <stdlib.h>

int_p float_quotient_int(const float_p num1, const int_p num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);
    res->ref = 0;

    mpfr_t resf;
    mpfr_init2(resf, float_prec_bit);

    mpfr_div_z(resf, num1->value, num2->value, MPFR_RNDN);

    mpfr_get_z(res->value, resf, MPFR_RNDD);

    mpfr_clear(resf);
    return res;
}
