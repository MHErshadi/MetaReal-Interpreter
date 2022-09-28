/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * -(num)
/*/

#include <complex.h>

void complex_negate(complex_p num)
{
    mpc_neg(num->value, num->value, MPC_RNDNN);
}
