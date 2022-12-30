/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/value.h>
#include <stdlib.h>
#include <string.h>

context_t context_set1(char* name, context_p parent, pos_p parent_pos, table_p table, const char* fname)
{
    context_t context;

    context.name = name;
    context.parent = parent;
    context.parent_pos = *parent_pos;
    context.table = *table;
    context.fname = fname;

    return context;
}

context_t context_set2(char* name, table_p table, const char* fname)
{
    context_t context;

    context.name = name;
    context.parent = NULL;
    context.table = *table;
    context.fname = fname;

    return context;
}

value_t context_var_get(context_p context, const char* name)
{
    value_t value = table_var_get(&context->table, name);

    if (value.type == NULL_V && context->parent)
        return context_var_get(context->parent, name);

    return value;
}

value_p context_ptr_get(context_p context, unsigned char* type, const char* name, char* flag)
{
    value_p value = table_ptr_get(&context->table, type, name, flag);

    while (!value && context->parent && !*flag)
    {
        context = context->parent;
        value = table_ptr_get(&context->table, type, name, flag);
    }

    return value;
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
        if (VAR_STATIC(table->vars[--table->size].properties))
            continue;

        value_delete(&table->vars[table->size].value);
        free(table->vars[table->size].name);
    }

    free(table->vars);
}

value_t table_var_get(table_p table, const char* name)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
            return table->vars[i].value;

    value_t null;
    null.type = NULL_V;
    return null;
}

value_p table_ptr_get(table_p table, unsigned char* type, const char* name, char* flag)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if (VAR_CONST(table->vars[i].properties) && table->vars[i].value.type != NULL_V)
            {
                *flag = 1;
                return NULL;
            }

            *type = table->vars[i].type;
            return &table->vars[i].value;
        }

    return NULL;
}

char table_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if ((VAR_CONST(table->vars[i].properties) && table->vars[i].value.type != NULL_V))
                return -1;

            if (table->vars[i].type != NULL_V && value->type != table->vars[i].type)
                return table->vars[i].type;

            value_delete(&table->vars[i].value);

            table->vars[i].properties |= properties;
            if (table->vars[i].type == NULL_V)
                table->vars[i].type = type;

            value->should_free = 0;

            table->vars[i].value = *value;
            return 0;
        }

    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    value->should_free = 0;

    table->vars[table->size] = (var_t){properties, copy, type, *value};
    return 0;
}

value_p table_ptr_set(table_p table, unsigned char* ptype, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if ((VAR_CONST(table->vars[i].properties) && table->vars[i].value.type != NULL_V) ||
                (VAR_CONST(properties) && value->type != NULL_V))
            {
                *flag = -1;
                return NULL;
            }

            if (table->vars[i].type != NULL_V && value->type != table->vars[i].type)
            {
                *flag = table->vars[i].type;
                return NULL;
            }

            value_delete(&table->vars[i].value);

            table->vars[i].properties |= properties;
            if (table->vars[i].type == NULL_V)
                table->vars[i].type = type;

            value->should_free = 0;

            table->vars[i].value = *value;

            if (ptype)
                *ptype = table->vars[i].type;
            return &table->vars[i].value;
        }

    if (VAR_CONST(properties) && value->type != NULL_V)
    {
        *flag = -1;
        return NULL;
    }

    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    value->should_free = 0;

    table->vars[table->size] = (var_t){properties, copy, type, *value};

    if (ptype)
        *ptype = type;
    return &table->vars[table->size++].value;
}

var_p table_ptr_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if ((VAR_CONST(table->vars[i].properties) && table->vars[i].value.type != NULL_V) ||
                (VAR_CONST(properties) && value->type != NULL_V))
            {
                *flag = -1;
                return NULL;
            }

            if (table->vars[i].type != NULL_V && value->type != table->vars[i].type)
            {
                *flag = table->vars[i].type;
                return NULL;
            }

            value_delete(&table->vars[i].value);

            table->vars[i].properties |= properties;
            if (table->vars[i].type == NULL_V)
                table->vars[i].type = type;

            value->should_free = 0;

            table->vars[i].value = *value;

            return table->vars + i;
        }

    if (VAR_CONST(properties) && value->type != NULL_V)
    {
        *flag = -1;
        return NULL;
    }

    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    value->should_free = 0;

    table->vars[table->size] = (var_t){properties, copy, type, *value};

    return table->vars + table->size++;
}

value_p table_ptr_add(table_p table, const char* name)
{
    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    value_t value;
    value.type = NULL_V;

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    table->vars[table->size] = (var_t){0, copy, NULL_V, value};
    table->vars[table->size].value.should_free = 0;

    return &table->vars[table->size++].value;
}
