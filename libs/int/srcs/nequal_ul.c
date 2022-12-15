/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * (num1) != (num2)
/*/

#include <int.h>

char int_nequal_ul(const int_p num1, unsigned long num2)
{
    return mpz_cmp_ui(num1->value, num2) != 0;
}
