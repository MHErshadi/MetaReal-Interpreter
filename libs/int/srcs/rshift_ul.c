/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) >> (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_rshift_ul(const int_p num1, unsigned long num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);

    mpz_fdiv_q_2exp(res->value, num1->value, num2);

    return res;
}

void int_rshift_ul_self(int_p num1, unsigned long num2)
{
    mpz_fdiv_q_2exp(num1->value, num1->value, num2);
}
