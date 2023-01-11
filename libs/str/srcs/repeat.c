/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) * (count)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_repeat(const str_p str, unsigned long long count)
{
    str_p res = malloc(sizeof(str_t));

    if (!count || !str->size)
    {
        res->str = malloc(1);
        *res->str = '\0';

        res->size = 0;

        return res;
    }

    res->size = str->size * count;
    res->str = malloc(res->size + 1);

    unsigned long long i, j;
    for (i = 0; i < res->size;)
        for (j = 0; j < str->size; i++, j++)
            res->str[i] = str->str[j];

    res->str[i] = '\0';

    return res;
}

void str_repeat_self(str_p str, unsigned long long count)
{
    if (!str->size || count == 1)
        return;

    if (!count)
    {
        free(str->str);
        str->size = 0;
        return;
    }

    unsigned long long size = str->size * count;
    str->str = realloc(str->str, size + 1);

    unsigned long long i, j;
    for (i = str->size; i < size;)
        for (j = 0; j < str->size; i++, j++)
            str->str[i] = str->str[j];

    str->str[i] = '\0';
    str->size = size;
}
