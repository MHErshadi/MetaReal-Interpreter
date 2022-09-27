/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 * 
 * Frees (num) from (cellular) and its data from (heap)
/*/

#include <float.h>

void float_free(float_p num, cellular_p cellular, heap_p heap)
{
    mpfr_clear(num->value);
    cellular_free(cellular, num);
}
