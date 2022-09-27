/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) >> (num2)
/*/

#include <int.h>

void int_rshift(int_p num1, int_p num2, heap_p heap)
{
    mpz_fdiv_q_2exp(num1->value, num1->value, mpz_get_ui(num2->value));
}
