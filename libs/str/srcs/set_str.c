/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Copies (src) based on (size)
/*/

#include <str.h>
#include <stdlib.h>

str_p str_set_str(const char* src, unsigned long long size)
{
    str_p dst = malloc(sizeof(str_t));

    dst->str = malloc(size + 1);

    unsigned long long i;
    for (i = 0; i <= size; i++)
        dst->str[i] = src[i];

    dst->size = size;

    return dst;
}
