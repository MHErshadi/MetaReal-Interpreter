/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) ** (num2)
/*/

#include <int.h>

void int_ul_power(unsigned long num1, int_p num2)
{
    mpz_ui_pow_ui(num2->value, num1, mpz_get_ui(num2->value));
}
