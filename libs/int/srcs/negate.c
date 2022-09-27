/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * -(num)
/*/

#include <int.h>

void int_negate(int_p num)
{
    mpz_neg(num->value, num->value);
}
