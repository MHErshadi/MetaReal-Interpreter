/*/
 * MetaReal version 1.0.0
/*/

#include <setting.h>

void setting_init()
{
    setting.output = stdout;
    setting.input = stdin;
    setting.error = stderr;

    setting.float_prec_bit = 64;
    setting.float_prec_show = 16;

    setting.complex_prec_bit = 64;
    setting.complex_prec_show = 16;
}
