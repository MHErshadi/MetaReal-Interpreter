/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Copies (src)
/*/

#include <int.h>
#include <memory.h>

int_p int_set(int_p src)
{
    int_p dst = cellular_alloc(&memory.int_cellular);

    mpz_init_set(dst->value, src->value);

    return dst;
}
