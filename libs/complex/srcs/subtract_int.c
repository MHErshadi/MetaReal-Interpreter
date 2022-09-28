/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 * 
 * (num1) - (num2)
/*/

#include <complex.h>

void complex_subtract_int(complex_p num1, int_p num2, heap_p heap)
{
    mpfr_t num2f;
    mpfr_init2(num2f, mpfr_get_prec(mpc_realref(num1->value)));
    mpfr_set_z(num2f, num2->value, MPFR_RNDN);

    mpc_sub_fr(num1->value, num1->value, num2f, MPC_RNDNN);

    mpfr_clear(num2f);
}
