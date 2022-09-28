/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * Converts (str) into complex based on (size)
/*/

#include <complex.h>

complex_p complex_set_str(unsigned long long prec, const char* str, unsigned long long size, cellular_p cellular, heap_p heap)
{
    complex_p dst = cellular_alloc(cellular);

    mpc_init3(dst->value, prec, prec);
    mpfr_set_zero(mpc_realref(dst->value), 0);
    mpfr_set_str(mpc_imagref(dst->value), str, 10, MPFR_RNDN);

    return dst;
}
