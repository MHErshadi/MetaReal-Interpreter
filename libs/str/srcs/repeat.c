/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) * (count)
/*/

#include <str.h>
#include <stdlib.h>

void str_repeat(str_p str, unsigned long long count)
{
    if (!count)
    {
        str->str = realloc(str->str, 1);
        *str->str = '\0';

        str->size = 0;
        return;
    }

    if (count == 1)
        return;

    if (!str->size)
        return;

    unsigned long long bsize = str->size;

    str->size *= count;
    str->str = realloc(str->str, str->size + 1);

    unsigned long long i, j;
    for (i = bsize; i < str->size;)
        for (j = 0; j < bsize; i++, j++)
            str->str[i] = str->str[j];
    str->str[i] = '\0';
}
