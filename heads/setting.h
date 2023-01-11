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
};
typedef struct __setting__ setting_t;
typedef struct __setting__* setting_p;

void setting_init();

setting_t setting;

#endif /* __M_SETTING__ */
