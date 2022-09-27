/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Prints (str) into (stream)
/*/

#include <str.h>

void str_print(FILE* stream, str_p str, cellular_p cellular, heap_p heap)
{
    fprintf(stream, "%s\n", str->str);

    heap_free(heap, str->str, str->size + 1);
    cellular_free(cellular, str);
}
