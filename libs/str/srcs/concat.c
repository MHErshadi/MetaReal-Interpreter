/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str1) + (str2)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_concat(const str_p str1, const str_p str2)
{
    str_p res = malloc(sizeof(str_t));

    res->size = str1->size + str2->size;
    res->str = malloc(res->size + 1);

    unsigned long long i;
    for (i = 0; i < str1->size; i++)
        res->str[i] = str1->str[i];

    unsigned long long j;
    for (j = 0; j <= str2->size; i++, j++)
        res->str[i] = str2->str[j];

    return res;
}

void str_concat_self(str_p str1, str_p str2)
{
    if (!str2->size)
        return;

    if (!str1->size)
    {
        str1->str = str2->str;
        str1->size = str2->size;

        str2->size = 0;
        return;
    }

    unsigned long long size = str1->size + str2->size;
    str1->str = realloc(str1->str, size + 1);

    unsigned long long i, j;
    for (i = str1->size, j = 0; j <= str2->size; i++, j++)
        str1->str[i] = str2->str[j];

    str1->size = size;
}
