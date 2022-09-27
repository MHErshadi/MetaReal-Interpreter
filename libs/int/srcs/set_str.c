/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (str) into integer based on (size)
/*/

#include <int.h>

int_p int_set_str(const char* str, unsigned long long size, cellular_p cellular, heap_p heap)
{
    int_p dst = cellular_alloc(cellular);

    mpz_init_set_str(dst->value, str, 10);

    return dst;
}
