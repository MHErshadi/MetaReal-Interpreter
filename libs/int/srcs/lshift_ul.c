/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) << (num2)
/*/

#include <int.h>

void int_lshift_ul(int_p num1, unsigned long num2)
{
    mpz_mul_2exp(num1->value, num1->value, num2);
}
