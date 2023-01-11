/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Frees (num)
/*/

#include <complex.h>
#include <stdlib.h>

void complex_free(complex_p num)
{
    mpc_clear(num->value);
    free(num);
}
