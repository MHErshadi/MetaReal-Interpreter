/*/
 * MetaReal version 1.0.0
/*/

#include <debugger/pos.h>

pos_t pos_set(unsigned long long index, unsigned long long line)
{
    pos_t pos;

    pos.index = index;
    pos.line = line;

    return pos;
}
