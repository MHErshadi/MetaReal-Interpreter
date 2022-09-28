/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_SETTING__
#define __M_SETTING__

#include <stdio.h>

struct __setting__
{
    FILE* output;
    FILE* input;
    FILE* error;

    unsigned long long float_prec_bit;
    unsigned long long float_prec_show;

    unsigned long long complex_prec_bit;
    unsigned long long complex_prec_show;
};
typedef struct __setting__ setting_t;
typedef struct __setting__* setting_p;

setting_t setting_init();

setting_t setting;

#endif /* __M_SETTING__ */
