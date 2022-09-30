/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Copies (src)
/*/

#include <str.h>
#include <memory.h>

str_p str_set(str_p src)
{
    str_p dst = cellular_alloc(&memory.str_cellular);

    dst->str = heap_alloc(&memory.heap, src->size + 1);
    dst->size = src->size;

    unsigned long long i;
    for (i = 0; i <= dst->size; i++)
        dst->str[i] = src->str[i];

    return dst;
}
