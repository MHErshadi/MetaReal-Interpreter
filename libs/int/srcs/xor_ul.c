/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) ^ (num2)
/*/

#include <int.h>

void int_xor_ul(int_p num1, unsigned long num2)
{
    mpz_t num2i;
    mpz_init_set_ui(num2i, num2);

    mpz_xor(num1->value, num1->value, num2i);

    mpz_clear(num2i);
}
