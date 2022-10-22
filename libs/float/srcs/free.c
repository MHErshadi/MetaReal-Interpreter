/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Frees (num) from (cellular) and its data from (heap)
/*/

#include <float.h>
#include <stdlib.h>

void float_free(float_p num)
{
    mpfr_clear(num->value);
    free(num);
}
