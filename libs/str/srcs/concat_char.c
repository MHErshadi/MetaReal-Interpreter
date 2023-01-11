/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) + (chr)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_concat_char(const str_p str, char chr)
{
    str_p res = malloc(sizeof(str_t));

    res->size = str->size + 1;
    res->str = malloc(res->size + 1);

    unsigned long long i;
    for (i = 0; i < str->size; i++)
        res->str[i] = str->str[i];

    res->str[i++] = chr;
    res->str[i] = '\0';

    return res;
}

void str_concat_char_self(str_p str, char chr)
{
    if (!str->size)
    {
        str->str = malloc(2);
        str->size = 1;

        *str->str = chr;
        str->str[1] = '\0';

        return;
    }

    str->str = realloc(str->str, str->size + 2);
    str->str[str->size++] = chr;
    str->str[str->size] = '\0';
}
