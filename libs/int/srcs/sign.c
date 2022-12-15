/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Returns sign of (num)
 * 1 if positive, 0 if zero and -1 if negative
/*/

#include <int.h>

char int_sign(const int_p num)
{
    return mpz_sgn(num->value);
}
