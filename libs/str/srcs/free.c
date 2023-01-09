/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Frees (str) from (cellular) and its data from (heap)
/*/

#include <str.h>
#include <stdlib.h>

void str_free(str_p str)
{
    if (str->ref)
    {
        str->ref--;
        return;
    }

    free(str->str);
    free(str);
}
