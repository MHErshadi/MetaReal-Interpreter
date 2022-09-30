/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) - (pos)
/*/

#include <str.h>
#include <memory.h>

void str_remove(str_p str, unsigned long long pos)
{
    unsigned long long i, j;
    for (i = pos, j = pos + 1; j <= str->size; i++, j++)
        str->str[i] = str->str[j];

    heap_shrink(&memory.heap, str->str, str->size + 1, str->size);
    str->size--;
}
