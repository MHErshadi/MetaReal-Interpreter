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
    res->ref = 0;

    mpz_and(res->value, num1->value, res->value);

    return res;
}
