/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Converts (str) into complex based on (size)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_set_str(const char* str1, unsigned long long size1,
    const char* str2, unsigned long long size2,
    unsigned char base)
{
    complex_p dst = malloc(sizeof(complex_t));

    mpc_init3(dst->value, complex_prec_bit, complex_prec_bit);

    if (size1)
        mpfr_set_str(mpc_realref(dst->value), str1, base, MPFR_RNDN);
    else
        mpfr_set_zero(mpc_realref(dst->value), 0);

    if (size2)
        mpfr_set_str(mpc_imagref(dst->value), str2, base, MPFR_RNDN);
    else
        mpfr_set_zero(mpc_imagref(dst->value), 0);

    dst->ref = 0;

    return dst;
}
