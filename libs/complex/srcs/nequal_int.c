/*/
 * MetaReal version 1.0.0
 *
 * Complex Library version 1.0.0
 *
 * (num1) != (num2)
/*/

#include <complex.h>

char complex_nequal_int(const complex_p num1, const int_p num2)
{
    mpc_t num2c;
    mpc_init3(num2c, complex_prec_bit, complex_prec_bit);
    mpc_set_z(num2c, num2->value, MPC_RNDNN);

    char res = mpc_cmp(num1->value, num2c) != 0;

    mpc_clear(num2c);
    return res;
}
