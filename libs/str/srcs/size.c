/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Returns size of (str)
/*/

#include <str.h>

unsigned long long str_size(const str_p str)
{
    return str->size;
}
