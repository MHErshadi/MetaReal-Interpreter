/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) + (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_add_int(const complex_p num1, const int_p num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);
    mpc_set_z(res->value, num2->value, MPC_RNDNN);

    mpc_add(res->value, num1->value, res->value, MPC_RNDNN);

    return res;
}

void complex_add_int_self(complex_p num1, const int_p num2)
{
    mpfr_t num2f;
    mpfr_init2(num2f, float_prec_bit);
    mpfr_set_z(num2f, num2->value, MPFR_RNDN);

    mpc_add_fr(num1->value, num1->value, num2f, MPC_RNDNN);

    mpfr_clear(num2f);
}
