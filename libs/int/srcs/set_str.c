/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (str) into integer based on (size)
/*/

#include <int.h>
#include <stdlib.h>

int_p int_set_str(const char* str, unsigned long long size, unsigned char base)
{
    int_p dst = malloc(sizeof(int_t));

    mpz_init_set_str(dst->value, str, base);
    dst->ref = 0;

    return dst;
}
