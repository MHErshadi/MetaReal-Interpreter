/*/
 * MetaReal version 1.0.0
 *
 * Integer Library version 1.0.0
 *
 * Converts (src) into str
/*/

#include <int.h>

char* int_get_str(int_p src)
{
    return mpz_get_str(NULL, 10, src->value);
}
