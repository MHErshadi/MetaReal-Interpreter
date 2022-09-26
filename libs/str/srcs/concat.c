/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Concatenates (str2) to (str1)
/*/

#include <str.h>

void str_concat(str_p str1, str_p str2, cellular_p cellular, heap_p heap)
{
    unsigned long long bsize = str1->size;

    str1->size += str2->size;
    str1->str = heap_expand(heap, str1->str, bsize + 1, str1->size + 1);

    unsigned long long i, j;
    for (i = bsize - 1, j = 0; j <= str2->size; i++, j++)
        str1->str[i] = str2->str[j];

    heap_free(heap, str2->str, str2->size + 1);
    cellular_free(cellular, str2);
}
