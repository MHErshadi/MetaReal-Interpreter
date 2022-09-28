/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * (num1) / (num2)
/*/

#include <complex.h>

void complex_int_divide(int_p num1, complex_p num2, heap_p heap)
{
    mpfr_t num1f;
    mpfr_init2(num1f, mpfr_get_prec(mpc_realref(num2->value)));
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpc_fr_div(num2->value, num1f, num2->value, MPC_RNDNN);

    mpfr_clear(num1f);
}
