/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * (str) + (chr)
/*/

#include <str.h>
#include <string.h>
#include <stdlib.h>

void str_concat_char(str_p str, char chr)
{
    str->str = realloc(str->str, str->size + 1);

    str->str[str->size++] = chr;
    str->str[str->size] = '\0';
}
