/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Copies (src)
/*/

#include <float.h>
#include <memory.h>

float_p float_set(unsigned long long prec, float_p src)
{
    float_p dst = cellular_alloc(&memory.float_cellular);

    mpfr_init2(dst->value, prec);
    mpfr_set(dst->value, src->value, MPFR_RNDN);

    return dst;
}
