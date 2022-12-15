/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <float.h>

void float_modulo_ul(float_p num1, unsigned long num2)
{
    mpfr_t num2f;
    mpfr_init2(num2f, mpfr_get_prec(num1->value));
    mpfr_set_ui(num2f, num2, MPFR_RNDN);

    mpfr_remainder(num1->value, num1->value, num2f, MPFR_RNDN);

    mpfr_clear(num2f);
}
