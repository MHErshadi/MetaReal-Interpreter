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

void str_concat_str(str_p str1, const char* str2)
{
    unsigned long long bsize = str1->size;
    unsigned long long str2_size = strlen(str2);

    str1->size += str2_size;
    str1->str = realloc(str1->str, str1->size + 1);

    unsigned long long i, j;
    for (i = bsize, j = 0; j <= str2_size; i++, j++)
        str1->str[i] = str2[j];
}
