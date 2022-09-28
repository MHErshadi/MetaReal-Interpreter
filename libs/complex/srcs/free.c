/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * Frees (num) from (cellular) and its data from (heap)
/*/

#include <complex.h>

void complex_free(complex_p num, cellular_p cellular, heap_p heap)
{
    mpc_clear(num->value);
    cellular_free(cellular, num);
}
