/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_divide_ul(const complex_p num1, unsigned long num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);

    mpc_div_ui(res->value, num1->value, num2, MPC_RNDNN);

    return res;
}

void complex_divide_ul_self(complex_p num1, unsigned long num2)
{
    mpc_div_ui(num1->value, num1->value, num2, MPC_RNDNN);
}
