/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/value.h>
#include <stdlib.h>

context_t context_set1(const char* name, context_p parent, pos_p parent_pos, table_p table, const char* fname)
{
    context_t context;

    context.name = name;
    context.parent = parent;
    context.parent_pos = *parent_pos;
    context.table = *table;
    context.fname = fname;

    return context;
}

context_t context_set2(const char* name, table_p table, const char* fname)
{
    context_t context;

    context.name = name;
    context.parent = NULL;
    context.table = *table;
    context.fname = fname;

    return context;
}

table_t table_set(unsigned long long alloc)
{
    table_t table;

    table.vars = malloc(alloc * sizeof(var_t));
    table.alloc = alloc;
    table.size = 0;

    return table;
}

void table_free(table_p table)
{
    while (table->size)
    {
        value_free(&table->vars[--table->size].value);
        free(table->vars[table->size].name);
    }
    free(table->vars);
}
