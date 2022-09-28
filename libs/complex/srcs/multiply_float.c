/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * (num1) * (num2)
/*/

#include <complex.h>

void complex_multiply_float(complex_p num1, float_p num2, heap_p heap)
{
    mpc_mul_fr(num1->value, num1->value, num2->value, MPC_RNDNN);
}
