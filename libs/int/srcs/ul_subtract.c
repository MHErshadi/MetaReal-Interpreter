/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) - (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_ul_subtract(unsigned long num1, const int_p num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);

    mpz_ui_sub(res->value, num1, num2->value);

    return res;
}

void int_ul_subtract_self(unsigned long num1, int_p num2)
{
    mpz_ui_sub(num2->value, num1, num2->value);
}
