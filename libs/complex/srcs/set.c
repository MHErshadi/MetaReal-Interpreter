/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Copies (src)
/*/

#include <complex.h>

complex_p complex_set(unsigned long long prec, complex_p src, cellular_p cellular, heap_p heap)
{
    complex_p dst = cellular_alloc(cellular);

    mpc_init3(dst->value, prec, prec);
    mpc_set(dst->value, src->value, MPC_RNDNN);

    return dst;
}
