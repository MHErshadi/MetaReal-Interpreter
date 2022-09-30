/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * ~(num)
/*/

#include <int.h>

void int_not(int_p num)
{
    mpz_com(num->value, num->value);
}
