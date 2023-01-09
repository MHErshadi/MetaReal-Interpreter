/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str2) in (str1)
/*/

#include <str.h>
#include <string.h>

char str_contains_str(const str_p str1, const char* str2)
{
    unsigned long long str2_size = strlen(str2);
    if (!str2_size)
        return 1;

    if (str1->size < str2_size)
        return 0;

    unsigned long long i, j;
    for (i = 0; i < str1->size; i++)
    {
        if (str1->str[i] == *str2)
        {
            for (j = 1; j < str2_size && i < str1->size; j++)
                if (str1->str[i + j] != str2[j])
                    break;

            if (j == str2_size)
                return 1;
        }
    }

    return 0;
}
