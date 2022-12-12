/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (chr) + (str)
/*/

#include <str.h>
#include <string.h>
#include <stdlib.h>

void str_char_concat(char chr, str_p str)
{
    unsigned long long size = str->size + 1;

    char* res = malloc(size + 1);

    unsigned long long i;
    *res = chr;
    for (i = 0; i <= str->size; i++)
        res[i + 1] = str->str[i];

    free(str->str);

    str->size = size;
    str->str = res;
}
