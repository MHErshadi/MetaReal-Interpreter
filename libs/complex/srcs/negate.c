/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * -(num)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_negate(const complex_p num)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);

    mpc_neg(res->value, num->value, MPC_RNDNN);

    return res;
}

void complex_negate_self(complex_p num)
{
    mpc_neg(num->value, num->value, MPC_RNDNN);
}
