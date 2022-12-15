/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <float.h>

void float_ul_modulo(unsigned long num1, float_p num2)
{
    mpfr_t num1f;
    mpfr_init2(num1f, mpfr_get_prec(num2->value));
    mpfr_set_ui(num1f, num1, MPFR_RNDN);

    mpfr_fmod(num2->value, num1f, num2->value, MPFR_RNDN);

    mpfr_clear(num1f);
}
