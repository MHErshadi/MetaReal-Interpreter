/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (src) into integer
/*/

#include <int.h>
#include <stdlib.h>

int_p int_set_ull(unsigned long long src)
{
    int_p dst = malloc(sizeof(int_t));

    mpz_import(dst->value, 1, -1, sizeof(unsigned long long), 0, 0, &src);

    return dst;
}
