/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Copies (src)
/*/

#include <float.h>

float_p float_set(unsigned long long prec, float_p src, cellular_p cellular, heap_p heap)
{
    float_p dst = cellular_alloc(cellular);

    mpfr_init2(dst->value, prec);
    mpfr_set(dst->value, src->value, MPFR_RNDN);

    return dst;
}
