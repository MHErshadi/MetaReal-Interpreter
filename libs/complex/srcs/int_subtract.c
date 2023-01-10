/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_int_subtract(const int_p num1, const complex_p num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);
    mpc_set_z(res->value, num1->value, MPC_RNDNN);

    res->ref = 0;

    mpc_sub(res->value, res->value, num2->value, MPC_RNDNN);

    return res;
}
