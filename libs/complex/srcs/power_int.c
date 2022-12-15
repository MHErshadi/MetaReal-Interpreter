/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <complex.h>

void complex_power_int(complex_p num1, const int_p num2)
{
    mpc_pow_z(num1->value, num1->value, num2->value, MPC_RNDNN);
}
