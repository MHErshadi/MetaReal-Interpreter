/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) - (pos)
/*/

#include <str.h>
#include <stdlib.h>

void str_remove(str_p str, unsigned long long pos)
{
    if (str->size == 1)
    {
        str->str = realloc(str->str, 1);
        *str->str = '\0';

        str->size = 0;
        return;
    }

    unsigned long long i, j;
    for (i = pos, j = pos + 1; j <= str->size; i++, j++)
        str->str[i] = str->str[j];

    str->str = realloc(str->str, str->size--);
}
