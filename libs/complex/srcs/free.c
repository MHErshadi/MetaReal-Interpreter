/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Frees (num) from (cellular) and its data from (heap)
/*/

#include <complex.h>
#include <stdlib.h>

void complex_free(complex_p num)
{
    if (num->ref)
    {
        num->ref--;
        return;
    }

    mpc_clear(num->value);
    free(num);
}
