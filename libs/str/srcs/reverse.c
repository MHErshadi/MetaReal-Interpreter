/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Reverses order of characters in (str)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_reverse(const str_p str)
{
    str_p res = malloc(sizeof(str_t));

    res->size = str->size;
    res->ref = 0;

    if (!str->size)
    {
        res->str = malloc(1);
        *res->str = '\0';

        return res;
    }

    if (str->size == 1)
    {
        res->str = malloc(2);
        *res->str = *str->str;
        res->str[1] = '\0';

        return res;
    }

    res->str = malloc(str->size + 1);

    char* start = res->str;
    char* end = res->str + str->size - 1;

    while (start < end)
    {
        char temp = *start;

        *start++ = *end;
        *end-- = temp;
    }

    return res;
}
