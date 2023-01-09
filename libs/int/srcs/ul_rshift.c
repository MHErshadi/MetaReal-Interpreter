/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) >> (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_ul_rshift(unsigned long num1, const int_p num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init_set_ui(res->value, num1);
    res->ref = 0;

    mpz_fdiv_q_2exp(res->value, res->value, mpz_get_ui(num2->value));

    return res;
}
