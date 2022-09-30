/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Frees (str) from (cellular) and its data from (heap)
/*/

#include <str.h>
#include <memory.h>

void str_free(str_p str)
{
    heap_free(&memory.heap, str->str, str->size + 1);
    cellular_free(&memory.str_cellular, str);
}
