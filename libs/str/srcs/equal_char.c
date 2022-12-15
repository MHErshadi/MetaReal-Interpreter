/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) == (chr)
/*/

#include <str.h>

char str_equal_char(const str_p str, char chr)
{
    if (str->size != 1)
        return 0;

    return *str->str == chr;
}
