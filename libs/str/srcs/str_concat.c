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

str_p str_str_concat(const char* str1, const str_p str2)
{
    unsigned long long str1_size = strlen(str1);

    str_p res = malloc(sizeof(str_t));

    res->size = str1_size + str2->size;
    res->str = malloc(res->size + 1);

    unsigned long long i;
    for (i = 0; i < str1_size; i++)
        res->str[i] = str1[i];

    unsigned long long j;
    for (j = 0; j <= str2->size; i++, j++)
        res->str[i] = str2->str[j];

    return res;
}

void str_str_concat_self(char* str1, str_p str2)
{
    unsigned long long str1_size = strlen(str1);

    if (!str2->size)
    {
        str2->str = str1;
        str2->size = str1_size;
        return;
    }

    str2->size += str1_size;

    str1 = realloc(str1, str2->size + 1);

    unsigned long long i, j;
    for (i = str1_size, j = 0; i <= str2->size; i++, j++)
        str1[i] = str2->str[j];

    free(str2->str);
    str2->str = str1;
}
