/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) * (num2)
/*/

#include <complex.h>

void complex_multiply_ul(complex_p num1, unsigned long num2)
{
    mpc_mul_ui(num1->value, num1->value, num2, MPC_RNDNN);
}