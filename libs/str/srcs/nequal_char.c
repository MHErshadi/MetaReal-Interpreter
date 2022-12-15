/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) != (chr)
/*/

#include <str.h>

char str_nequal_char(const str_p str, char chr)
{
    if (str->size != 1)
        return 1;

    return *str->str != chr;
}
