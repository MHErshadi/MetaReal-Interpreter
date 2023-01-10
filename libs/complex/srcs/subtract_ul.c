/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_subtract_ul(const complex_p num1, unsigned long num2)
{
    complex_p res = malloc(sizeof(complex_t));

    mpc_init3(res->value, complex_prec_bit, complex_prec_bit);
    res->ref = 0;

    mpc_sub_ui(res->value, num1->value, num2, MPC_RNDNN);

    return res;
}
