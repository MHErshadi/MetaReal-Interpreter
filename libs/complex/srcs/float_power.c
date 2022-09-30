/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <complex.h>

void complex_float_power(float_p num1, complex_p num2)
{
    mpc_t num1c;
    mpc_init3(num1c, mpfr_get_prec(mpc_realref(num2->value)), mpfr_get_prec(mpc_imagref(num2->value)));
    mpc_set_fr(num1c, num1->value, MPC_RNDNN);

    mpc_pow(num2->value, num1c, num2->value, MPC_RNDNN);

    mpc_clear(num1c);
}
