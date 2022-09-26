/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Compares (str1) and (str2), returns 1 if they were equal and 0 otherwise
/*/

#include <str.h>

char str_equal(str_p str1, str_p str2, cellular_p cellular, heap_p heap)
{
    char res = 0;

    if (str1->size != str2->size)
        goto free;

    unsigned long long i;
    for (i = 0; i < str1->size; i++)
        if (str1->str[i] != str2->str[i])
            goto free;
    res = 1;

free:
    cellular_free(cellular, str2);
    heap_free(heap, str2->str, str2->size + 1);

    cellular_free(cellular, str1);
    heap_free(heap, str1->str, str1->size + 1);

    return res;
}
