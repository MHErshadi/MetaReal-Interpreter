/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <complex.h>
#include <memory.h>

complex_p complex_float_power_float(float_p num1, float_p num2, unsigned long long prec)
{
    complex_p res = cellular_alloc(&memory.complex_cellular);

    mpc_init3(res->value, prec, prec);
    mpc_set_fr(res->value, num1->value, MPC_RNDNN);

    mpc_pow_fr(res->value, res->value, num2->value, MPC_RNDNN);

    return res;
}
