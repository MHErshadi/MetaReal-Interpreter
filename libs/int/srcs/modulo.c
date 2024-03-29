/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_modulo(const int_p num1, const int_p num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);

    mpz_mod(res->value, num1->value, num2->value);

    return res;
}

void int_modulo_self(int_p num1, const int_p num2)
{
    mpz_mod(num1->value, num1->value, num2->value);
}
