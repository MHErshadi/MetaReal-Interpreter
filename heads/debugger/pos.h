/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_POS__
#define __M_POS__

struct __pos__
{
    unsigned long long index;
    unsigned long long line;
};
typedef struct __pos__ pos_t;
typedef struct __pos__* pos_p;

pos_t pos_set(unsigned long long index, unsigned long long line);

#endif /* __M_POS__ */
