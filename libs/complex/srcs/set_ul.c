/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * Converts (src) into complex
/*/

#include <complex.h>
#include <stdlib.h>

complex_p complex_set_ul(unsigned long src, unsigned long long prec)
{
    complex_p dst = malloc(sizeof(complex_t));

    mpc_init3(dst->value, prec, prec);
    mpc_set_ui(dst->value, src, MPC_RNDNN);

    return dst;
}
