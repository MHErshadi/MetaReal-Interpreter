/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (src) into str
/*/

#include <int.h>

char* int_get_str(const int_p src, unsigned char base)
{
    return mpz_get_str(NULL, base, src->value);
}
