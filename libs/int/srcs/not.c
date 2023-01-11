/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * ~(num)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_not(const int_p num)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);

    mpz_com(res->value, num->value);

    return res;
}

void int_not_self(int_p num)
{
    mpz_com(num->value, num->value);
}
