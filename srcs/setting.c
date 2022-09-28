/*/
 * MetaReal version 1.0.0
/*/

#include <setting.h>

setting_t setting_init()
{
    setting_t setting;

    setting.output = stdout;
    setting.input = stdin;
    setting.error = stderr;

    setting.float_prec_bit = 64;
    setting.float_prec_show = 16;

    setting.complex_prec_bit = 64;
    setting.complex_prec_show = 16;

    return setting;
}
