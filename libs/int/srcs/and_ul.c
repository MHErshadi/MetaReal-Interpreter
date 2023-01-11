/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) & (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_and_ul(const int_p num1, unsigned long num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init_set_ui(res->value, num2);

    mpz_and(res->value, num1->value, res->value);

    return res;
}

void int_and_ul_self(int_p num1, unsigned long num2)
{
    mpz_t num2i;
    mpz_init_set_ui(num2i, num2);

    mpz_and(num1->value, num1->value, num2i);

    mpz_clear(num2i);
}
