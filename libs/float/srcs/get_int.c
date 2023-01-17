/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Converts (src) into integer
/*/

#include <float.h>
#include <stdlib.h>

int_p float_get_int(const float_p src)
{
    int_p dst = malloc(sizeof(int_t));

    mpz_init(dst->value);
    mpfr_get_z(dst->value, src->value, MPFR_RNDD);

    return dst;
}
