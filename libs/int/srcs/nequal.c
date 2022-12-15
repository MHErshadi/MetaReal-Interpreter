/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) != (num2)
/*/

#include <int.h>

char int_nequal(const int_p num1, const int_p num2)
{
    return mpz_cmp(num1->value, num2->value) != 0;
}
