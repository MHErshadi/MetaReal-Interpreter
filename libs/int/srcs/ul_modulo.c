/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_ul_modulo(unsigned long num1, const int_p num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init_set_ui(res->value, num1);

    mpz_mod(res->value, res->value, num2->value);

    return res;
}

void int_ul_modulo_self(unsigned long num1, int_p num2)
{
    mpz_t num1i;
    mpz_init_set_ui(num1i, num1);

    mpz_mod(num2->value, num1i, num2->value);

    mpz_clear(num1i);
}
