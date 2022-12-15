/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) >= (num2)
/*/

#include <int.h>

char int_ul_greater_equal(unsigned long num1, const int_p num2)
{
    return mpz_cmp_ui(num2->value, num1) <= 0;
}
