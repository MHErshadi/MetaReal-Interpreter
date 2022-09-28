/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) != (num2)
/*/

#include <complex.h>

char complex_nequal(complex_p num1, complex_p num2)
{
    return mpc_cmp(num1->value, num2->value) != 0;
}
