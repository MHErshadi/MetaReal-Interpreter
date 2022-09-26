/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Checks if (str) contains (chr) or not, return 1 if it contains and 0 otherwise
/*/

#include <str.h>

char str_contains(str_p str, char chr, cellular_p cellular, heap_p heap)
{
    char res = 0;

    unsigned long long i;
    for (i = 0; i < str->size; i++)
        if (str->str[i] == chr)
        {
            res = 1;
            goto free;
        }

free:
    heap_free(heap, str->str, str->size + 1);
    cellular_free(cellular, str);

    return res;
}
