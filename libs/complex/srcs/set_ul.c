/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Converts (src) into complex
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_set_ul(unsigned long src)
{
    complex_p dst = malloc(sizeof(complex_t));

    mpc_init3(dst->value, complex_prec_bit, complex_prec_bit);
    mpc_set_ui(dst->value, src, MPC_RNDNN);

    return dst;
}
