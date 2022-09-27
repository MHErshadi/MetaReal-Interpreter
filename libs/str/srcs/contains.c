/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (chr) in (str)
/*/

#include <str.h>

char str_contains(str_p str, char chr)
{
    unsigned long long i;
    for (i = 0; i < str->size; i++)
        if (str->str[i] == chr)
            return 1;
    return 0;
}
