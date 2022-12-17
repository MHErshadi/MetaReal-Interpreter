/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Reverses order of characters in (str)
/*/

#include <str.h>
#include <stdlib.h>

void str_reverse(str_p str)
{
    if (str->size <= 1)
        return;

    char* start = str->str;
    char* end = str->str + str->size - 1;

    while (start < end)
    {
        char temp = *start;

        *start++ = *end;
        *end-- = temp;
    }
}
