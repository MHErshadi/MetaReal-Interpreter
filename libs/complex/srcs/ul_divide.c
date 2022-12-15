/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) / (num2)
/*/

#include <complex.h>

void complex_ul_divide(unsigned long num1, complex_p num2)
{
    mpc_ui_div(num2->value, num1, num2->value, MPC_RNDNN);
}
