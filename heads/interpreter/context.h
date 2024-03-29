/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_CONTEXT__
#define __M_CONTEXT__

#include "value.h"
#include <debugger/pos.h>
#include <stdio.h>

#define DEF_VAR 0b11101

#define VAR_PUBLIC(x) (x & 1)
#define VAR_CONST(x)  (x >> 2 & 1)
#define VAR_STATIC(x) (x >> 3 & 1)
#define VAR_DEFAULT(x) (x >> 4 & 1)

struct __var__
{
    unsigned char properties;
    char* name;

    unsigned char type;
    value_p value;
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

#define context_name(c) (((context_p)c)->name)

/* */

context_t context_set1(char* name, context_p parent, pos_p parent_pos, table_p table, const char* fname);
context_t context_set2(char* name, table_p table, const char* fname);

context_t context_copy_debug(const context_p context);

void context_free(context_p context);
void context_free_debug(context_p context);

void context_print(FILE* stream, const char* label, const context_p context, const char* end);

value_p context_var_get(context_p context, const char* name);
var_p context_ptr_get(context_p context, const char* name, char* flag);

/* */

table_t table_set(unsigned long long alloc);

table_t table_set_def();

void table_delete(table_p table);
void table_free(table_p table);

value_p table_var_get(table_p table, const char* name);
var_p table_ptr_get(table_p table, const char* name, char* flag);

char table_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value);
var_p table_ptr_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag);
var_p table_ptr_add(table_p table, const char* name);

#endif /* __M_CONTEXT__ */
