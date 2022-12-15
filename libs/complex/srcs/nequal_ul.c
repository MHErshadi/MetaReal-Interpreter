/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) != (num2)
/*/

#include <complex.h>

char complex_nequal_ul(const complex_p num1, unsigned long num2)
{
    mpc_t num2c;
    mpc_init3(num2c, mpfr_get_prec(mpc_realref(num1->value)), mpfr_get_prec(mpc_imagref(num1->value)));
    mpc_set_ui(num2c, num2, MPC_RNDNN);

    char res = mpc_cmp(num1->value, num2c) != 0;

    mpc_clear(num2c);

    return res;
}
