/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * Prints (num) + (end) into (stream)
/*/

#include <complex.h>

void complex_print(FILE* stream, complex_p num, unsigned long long prec, const char* end)
{
    if (mpfr_sgn(mpc_realref(num->value)))
        mpfr_fprintf(stream, "(%.*Rg%+.*Rgi)%s", prec, mpc_realref(num->value), prec, mpc_imagref(num->value), end);
    else
        mpfr_fprintf(stream, "%.*Rgi%s", prec, mpc_imagref(num->value), end);
}
