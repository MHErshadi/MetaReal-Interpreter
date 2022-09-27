/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (src) into integer
/*/

#include <int.h>

int_p int_set_ull(unsigned long long src, cellular_p cellular, heap_p heap)
{
    int_p dst = cellular_alloc(cellular);

    mpz_import(dst->value, 1, -1, sizeof(unsigned long long), 0, 0, &src);

    return dst;
}
