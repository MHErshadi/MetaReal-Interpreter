/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Copies (src) into (dst) based on (size)
/*/

#include <str.h>

str_p str_set_str(const char* src, unsigned long long size, cellular_p cellular, heap_p heap)
{
    str_p dst = cellular_alloc(cellular);

    dst->str = heap_alloc(heap, size + 1);
    dst->size = size;

    unsigned long long i;
    for (i = 0; i <= size; i++)
        dst->str[i] = src[i];

    return dst;
}
