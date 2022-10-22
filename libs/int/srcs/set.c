/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Copies (src)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_set(int_p src)
{
    int_p dst = malloc(sizeof(int_t));

    mpz_init_set(dst->value, src->value);

    return dst;
}
