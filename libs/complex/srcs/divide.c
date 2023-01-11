/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_divide(const complex_p num1, const complex_p num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);

    mpc_div(res->value, num1->value, num2->value, MPC_RNDNN);

    return res;
}

void complex_divide_self(complex_p num1, const complex_p num2)
{
    mpc_div(num1->value, num1->value, num2->value, MPC_RNDNN);
}
