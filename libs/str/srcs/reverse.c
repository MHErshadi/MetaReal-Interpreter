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

    unsigned long long i, j;
    for (i = 0, j = str->size - 1; i < str->size; i++, j--)
        res->str[i] = str->str[j];
    res->str[i] = '\0';

    return res;
}

void str_reverse_self(str_p str)
{
    if (str->size <= 1)
        return;

    char* start = str->str;
    char* end = str->str + str->size - 1;

    char tmp;
    while (start < end)
    {
        tmp = *start;

        *start++ = *end;
        *end-- = tmp;
    }
}
