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

void str_str_concat(const char* str1, str_p str2)
{
    unsigned long long str1_size = strlen(str1);
    unsigned long long size = str1_size + str2->size;

    char* res = malloc(size + 1);

    unsigned long long i, j;
    for (i = 0; i < str1_size; i++)
        res[i] = str1[i];
    for (j = 0; j <= str2->size; i++, j++)
        res[i] = str2->str[j];

    free(str2->str);

    str2->size = size;
    str2->str = res;
}
