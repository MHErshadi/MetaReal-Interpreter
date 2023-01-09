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
    res->ref = 0;

    unsigned long long i;
    for (i = 0; i < str1->size; i++)
        res->str[i] = str1->str[i];

    unsigned long long j;
    for (j = 0; j <= str2->size; i++, j++)
        res->str[i] = str2->str[j];

    return res;
}
