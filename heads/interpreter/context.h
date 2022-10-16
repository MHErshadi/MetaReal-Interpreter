/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_CONTEXT__
#define __M_CONTEXT__

#include <parser/node.h>

struct __context__
{
    const char* name;

    struct __context__* parent;
    pos_t parent_pos;

    const char* fname;
};
typedef struct __context__ context_t;
typedef struct __context__* context_p;

context_t context_set1(const char* name, context_p parent, pos_p parent_pos, const char* fname);
context_t context_set2(const char* name, const char* fname);

#endif /* __M_CONTEXT__ */
