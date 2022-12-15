/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str1) + (str2)
/*/

#include <str.h>
#include <stdlib.h>

void str_concat(str_p str1, const str_p str2)
{
    unsigned long long bsize = str1->size;

    str1->size += str2->size;
    str1->str = realloc(str1->str, str1->size + 1);

    unsigned long long i, j;
    for (i = bsize, j = 0; j <= str2->size; i++, j++)
        str1->str[i] = str2->str[j];
}
