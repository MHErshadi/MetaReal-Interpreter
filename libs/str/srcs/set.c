/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Copies (src) into (dst)
/*/

#include <str.h>

str_p str_set(str_p src, cellular_p cellular, heap_p heap)
{
    str_p dst = cellular_alloc(cellular);

    dst->str = heap_alloc(heap, src->size + 1);
    dst->size = src->size;

    unsigned long long i;
    for (i = 0; i <= dst->size; i++)
        dst->str[i] = src->str[i];

    return dst;
}
