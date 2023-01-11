/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_ul_subtract(unsigned long num1, const complex_p num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);

    mpc_ui_sub(res->value, num1, num2->value, MPC_RNDNN);

    return res;
}

void complex_ul_subtract_self(unsigned long num1, complex_p num2)
{
    mpc_ui_sub(num2->value, num1, num2->value, MPC_RNDNN);
}
