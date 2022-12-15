/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) | (num2)
/*/

#include <int.h>

void int_or(int_p num1, const int_p num2)
{
    mpz_ior(num1->value, num1->value, num2->value);
}
