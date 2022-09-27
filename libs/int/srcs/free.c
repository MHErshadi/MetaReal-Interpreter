/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Frees (num) from (cellular) and its data from (heap)
/*/

#include <int.h>

void int_free(int_p num, cellular_p cellular, heap_p heap)
{
    mpz_clear(num->value);
    cellular_free(cellular, num);
}
