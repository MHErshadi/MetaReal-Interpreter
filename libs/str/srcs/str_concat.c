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
    res->ref = 0;

    unsigned long long i;
    for (i = 0; i < str1_size; i++)
        res->str[i] = str1[i];

    unsigned long long j;
    for (j = 0; j <= str2->size; i++, j++)
        res->str[i] = str2->str[j];

    return res;
}
