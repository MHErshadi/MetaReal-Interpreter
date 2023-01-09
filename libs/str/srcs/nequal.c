/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str1) != (str2)
/*/

#include <str.h>

char str_nequal(const str_p str1, const str_p str2)
{
    if (str1->size != str2->size)
        return 1;

    unsigned long long i;
    for (i = 0; i < str1->size; i++)
        if (str1->str[i] != str2->str[i])
            return 1;

    return 0;
}
