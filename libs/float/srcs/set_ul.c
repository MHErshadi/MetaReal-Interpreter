/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * Converts (src) into float
/*/

#include <float.h>
#include <stdlib.h>

float_p float_set_ul(unsigned long src)
{
    float_p dst = malloc(sizeof(float_t));

    mpfr_init2(dst->value, float_prec_bit);
    mpfr_set_ui(dst->value, src, MPFR_RNDN);

    return dst;
}
