/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (chr) + (str)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_char_concat(char chr, const str_p str)
{
    str_p res = malloc(sizeof(str_t));

    res->size = str->size + 1;
    res->str = malloc(res->size + 1);

    *res->str = chr;
    unsigned long long i;
    for (i = 0; i <= str->size; i++)
        res->str[i + 1] = str->str[i];

    return res;
}

void str_char_concat_self(char chr, str_p str)
{
    if (!str->size)
    {
        str->str = malloc(2);
        str->size = 1;

        *str->str = chr;
        str->str[1] = '\0';

        return;
    }

    str->str = realloc(str->str, ++str->size + 1);

    char tmp;
    unsigned long long i;
    for (i = 0; i < str->size; i++)
    {
        tmp = str->str[i];

        str->str[i] = chr;
        chr = tmp;
    }
}
