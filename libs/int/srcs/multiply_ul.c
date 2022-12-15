/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) * (num2)
/*/

#include <int.h>

void int_multiply_ul(int_p num1, unsigned long num2)
{
    mpz_mul_ui(num1->value, num1->value, num2);
}
