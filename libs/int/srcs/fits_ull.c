/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Returns 1 if (num) fits in ull and 0 otherwise
/*/

#include <int.h>

char int_fits_ull(const int_p num)
{
    return mpz_size(num->value) <= 1;
}
