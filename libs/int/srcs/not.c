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
    res->ref = 0;

    mpz_com(res->value, num->value);

    return res;
}
