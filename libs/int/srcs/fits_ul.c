/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Returns 1 if (num) fits in ul and 0 otherwise
/*/

#include <int.h>

char int_fits_ul(const int_p num)
{
    return mpz_fits_ulong_p(num->value);
}
