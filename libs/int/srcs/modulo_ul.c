/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_modulo_ul(const int_p num1, unsigned long num2)
{
    int_p res = malloc(sizeof(int_t));

    mpz_init(res->value);
    res->ref = 0;

    mpz_mod_ui(res->value, num1->value, num2);

    return res;
}
