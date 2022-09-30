/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Copies (src) based on (size)
/*/

#include <str.h>
#include <memory.h>

str_p str_set_str(const char* src, unsigned long long size)
{
    str_p dst = cellular_alloc(&memory.str_cellular);

    dst->str = heap_alloc(&memory.heap, size + 1);
    dst->size = size;

    unsigned long long i;
    for (i = 0; i <= size; i++)
        dst->str[i] = src[i];

    return dst;
}
