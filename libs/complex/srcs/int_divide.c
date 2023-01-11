/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_int_divide(const int_p num1, const complex_p num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);
    mpc_set_z(res->value, num1->value, MPC_RNDNN);

    mpc_div(res->value, res->value, num2->value, MPC_RNDNN);

    return res;
}

void complex_int_divide_self(const int_p num1, complex_p num2)
{
    mpfr_t num1f;
    mpfr_init2(num1f, float_prec_bit);
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpc_fr_div(num2->value, num1f, num2->value, MPC_RNDNN);

    mpfr_clear(num1f);
}
