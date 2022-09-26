/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Frees (str) from (cellular) and its data from (heap)
/*/

#include <str.h>

void str_free(str_p str, cellular_p cellular, heap_p heap)
{
    heap_free(heap, str->str, str->size + 1);
    cellular_free(cellular, str);
}
