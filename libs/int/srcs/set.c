/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Copies (src)
/*/

#include <int.h>

int_p int_set(int_p src, cellular_p cellular, heap_p heap)
{
    int_p dst = cellular_alloc(cellular);

    mpz_init_set(dst->value, src->value);

    return dst;
}
