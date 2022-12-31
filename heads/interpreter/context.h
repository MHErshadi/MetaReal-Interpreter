/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_CONTEXT__
#define __M_CONTEXT__

#include "value.h"

#define VAR_PUBLIC(x) (x & 1)
#define VAR_CONST(x)  (x >> 2 & 1)
#define VAR_STATIC(x) (x >> 3 & 1)

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
    char* name;

    struct __context__* parent;
    pos_t parent_pos;

    table_t table;

    const char* fname;
};
typedef struct __context__ context_t;
typedef struct __context__* context_p;

context_t context_set1(char* name, context_p parent, pos_p parent_pos, table_p table, const char* fname);
context_t context_set2(char* name, table_p table, const char* fname);

value_t context_var_get(context_p context, const char* name);
value_p context_ptr_get(context_p context, unsigned char* type, const char* name, char* flag);

/* */

table_t table_set(unsigned long long alloc);

void table_free(table_p table);

value_t table_var_get(table_p table, const char* name);
value_p table_ptr_get(table_p table, unsigned char* type, const char* name, char* flag);

char table_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value);
value_p table_ptr_set(table_p table, unsigned char* ptype, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag);
var_p table_ptr_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag);

value_p table_ptr_add(table_p table, const char* name);

#endif /* __M_CONTEXT__ */
