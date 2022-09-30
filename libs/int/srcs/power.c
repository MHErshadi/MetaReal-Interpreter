/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <int.h>

void int_power(int_p num1, int_p num2)
{
    mpz_pow_ui(num1->value, num1->value, mpz_get_ui(num2->value));
}
