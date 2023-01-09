/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Frees (num) from (cellular) and its data from (heap)
/*/

#include <int.h>
#include <stdlib.h>

void int_free(int_p num)
{
    if (num->ref)
    {
        num->ref--;
        return;
    }

    mpz_clear(num->value);
    free(num);
}
