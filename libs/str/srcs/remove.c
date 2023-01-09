/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) - (pos)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_remove(const str_p str, unsigned long long pos)
{
    str_p res = malloc(sizeof(str_t));

    res->ref = 0;

    if (str->size == 1)
    {
        res->str = malloc(1);
        *res->str = '\0';

        res->size = 0;

        return res;
    }

    res->str = malloc(str->size);
    res->size = str->size - 1;

    unsigned long long i;
    for (i = 0; i < pos; i++)
        res->str[i] = str->str[i];
    for (; i < str->size; i++)
        res->str[i] = str->str[i + 1];

    return res;
}
