/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str2) in (str1)
/*/

#include <str.h>

char str_contains(const str_p str1, const str_p str2)
{
    if (!str2->size)
        return 1;

    if (str1->size < str2->size)
        return 0;

    unsigned long long i, j;
    for (i = 0; i < str1->size; i++)
    {
        if (str1->str[i] == *str2->str)
        {
            for (j = 1; j < str2->size && i < str1->size; j++)
                if (str1->str[i + j] != str2->str[j])
                    break;

            if (j == str2->size)
                return 1;
        }
    }

    return 0;
}
