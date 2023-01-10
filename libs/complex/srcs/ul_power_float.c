/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_ul_power_float(unsigned long num1, const float_p num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);
    mpc_set_ui(res->value, num1, MPC_RNDNN);

    res->ref = 0;

    mpc_pow_fr(res->value, res->value, num2->value, MPC_RNDNN);

    return res;
}
