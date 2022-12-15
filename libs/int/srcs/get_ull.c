/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (src) into ull
/*/

#include <int.h>

unsigned long long int_get_ull(const int_p src)
{
    unsigned long long dst = 0;
    mpz_export(&dst, 0, -1, sizeof(unsigned long long), 0, 0, src->value);

    return dst;
}
