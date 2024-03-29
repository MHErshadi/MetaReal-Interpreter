/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * -(num)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_negate(const int_p num)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);

    mpz_neg(res->value, num->value);

    return res;
}

void int_negate_self(int_p num)
{
    mpz_neg(num->value, num->value);
}
