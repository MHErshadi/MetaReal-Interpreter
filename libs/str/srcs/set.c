/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Copies (src)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_set(const str_p src)
{
    str_p dst = malloc(sizeof(str_t));

    dst->str = malloc(src->size + 1);

    unsigned long long i;
    for (i = 0; i <= src->size; i++)
        dst->str[i] = src->str[i];

    dst->size = src->size;

    return dst;
}
