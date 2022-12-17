/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_CONTEXT__
#define __M_CONTEXT__

#include <parser/node.h>

#define VAR_PUBLIC(x) (x & 1)
#define VAR_CONST(x)  (x >> 2 & 1)
#define VAR_STATIC(x) (x >> 3 & 1)

struct __value__
{
    unsigned char type;
    union_value_t value;

    pos_t poss;
    pos_t pose;

    struct __context__* context;
};
typedef struct __value__ value_t;
typedef struct __value__* value_p;

struct __var__
{
    unsigned char properties;
    char* name;

    unsigned char type;
    value_t value;
};
typedef struct __var__ var_t;
typedef struct __var__* var_p;

struct __table__
{
    var_p vars;
    unsigned long long alloc;
    unsigned long long size;
};
typedef struct __table__ table_t;
typedef struct __table__* table_p;

struct __context__
{
    const char* name;

    struct __context__* parent;
    pos_t parent_pos;

    table_t table;

    const char* fname;
};
typedef struct __context__ context_t;
typedef struct __context__* context_p;

context_t context_set1(const char* name, context_p parent, pos_p parent_pos, table_p table, const char* fname);
context_t context_set2(const char* name, table_p table, const char* fname);

value_t context_var_get(context_p context, const char* name);

/* */

table_t table_set(unsigned long long alloc);

void table_free(table_p table);

value_t table_var_get(table_p table, const char* name);
char table_var_set(table_p table, unsigned char properties, char* name, unsigned char type, value_p value);

#endif /* __M_CONTEXT__ */
