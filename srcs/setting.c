/*/
 * MetaReal version 1.0.0
/*/

#include <setting.h>
#include <complex.h>

void setting_init()
{
    setting.output = stdout;
    setting.input = stdin;
    setting.error = stderr;

    float_prec_bit = 64;
    float_prec_show = 16;

    complex_prec_bit = 64;
    complex_prec_show = 16;
}
