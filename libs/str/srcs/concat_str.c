/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str1) + (str2)
/*/

#include <str.h>
#include <string.h>
#include <stdlib.h>

str_p str_concat_str(const str_p str1, const char* str2)
{
    str_p res = malloc(sizeof(str_t));

    res->size = str1->size + strlen(str2);
    res->str = malloc(res->size + 1);

    unsigned long long i;
    for (i = 0; i < str1->size; i++)
        res->str[i] = str1->str[i];

    unsigned long long j;
    for (j = 0; i <= res->size; i++, j++)
        res->str[i] = str2[j];

    return res;
}

void str_concat_str_self(str_p str1, char* str2)
{
    if (!str1->size)
    {
        str1->str = str2;
        str1->size = strlen(str2);
        return;
    }

    unsigned long long size = str1->size + strlen(str2);
    str1->str = realloc(str1->str, size + 1);

    unsigned long long i, j;
    for (i = str1->size, j = 0; i <= size; i++, j++)
        str1->str[i] = str2[j];

    str1->size = size;

    free(str2);
}
