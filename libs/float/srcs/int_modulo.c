/*/
 * MetaReal version 1.0.0
 *
 * Float Library version 1.0.0
 *
 * (num1) % (num2)
/*/

#include <float.h>

void float_int_modulo(int_p num1, float_p num2, heap_p heap)
{
    mpfr_t num1f;
    mpfr_init2(num1f, mpfr_get_prec(num2->value));
    mpfr_set_z(num1f, num1->value, MPFR_RNDN);

    mpfr_fmod(num2->value, num1f, num2->value, MPFR_RNDN);

    mpfr_clear(num1f);
}
