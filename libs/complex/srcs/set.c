/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Copies (src)
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_set(const complex_p src)
{
    complex_p dst = malloc(sizeof(complex_t));

    mpc_init3(dst->value, complex_prec_bit, complex_prec_bit);
    mpc_set(dst->value, src->value, MPC_RNDNN);

    dst->ref = 0;

    return dst;
}
