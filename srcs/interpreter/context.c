/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/context.h>

context_t context_set1(const char* name, context_p parent, pos_p parent_pos, const char* fname)
{
    context_t context;

    context.name = name;
    context.parent = parent;
    context.parent_pos = *parent_pos;
    context.fname = fname;

    return context;
}

context_t context_set2(const char* name, const char* fname)
{
    context_t context;

    context.name = name;
    context.parent = NULL;
    context.fname = fname;

    return context;
}
