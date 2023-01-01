/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/context.h>
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

context_t context_copy(const context_p context)
{
    context_t copy;

    copy.name = malloc(strlen(context->name) + 1);
    strcpy(copy.name, context->name);

    copy.parent = context->parent;
    copy.parent_pos = context->parent_pos;

    copy.table = table_copy(&context->table);

    copy.fname = context->fname;

    return copy;
}

value_t context_var_get(context_p context, const char* name)
{
    value_t value = table_var_get(&context->table, name);

    if (value.type == NULL_V && context->parent)
        return context_var_get(context->parent, name);

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

table_t table_copy(const table_p table)
{
    table_t copy;

    copy.vars = malloc(table->alloc * sizeof(var_t));

    unsigned long long i;
    for (i = 0; i < table->size; i++)
    {
        copy.vars[i].properties = table->vars[i].properties;

        copy.vars[i].name = malloc(strlen(table->vars[i].name) + 1);
        copy.vars[i].name = table->vars[i].name;

        copy.vars[i].type = table->vars[i].type;

        copy.vars[i].value = value_copy(&copy.vars[i].value);
        copy.vars[i].value.should_free = 0;
    }

    copy.alloc = table->alloc;
    copy.size = table->size;

    return copy;
}

void table_delete(table_p table)
{
    while (table->size)
    {
        value_delete(&table->vars[table->size].value);
        free(table->vars[table->size].name);
    }

    free(table->vars);
}

void table_free(table_p table)
{
    unsigned long long i, j;
    for (i = 0, j = 0; i < table->size; i++)
    {
        if (VAR_STATIC(table->vars[i].properties))
        {
            if (i != j)
            {
                table->vars[j].properties = table->vars[i].properties;
                table->vars[j].name = table->vars[i].name;

                table->vars[j].type = table->vars[i].type;
                table->vars[j].value = table->vars[i].value;
            }

            j++;
            continue;
        }

        value_delete(&table->vars[i].value);
        free(table->vars[i].name);
    }

    table->size = j;
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

    table->vars[table->size++] = (var_t){properties, copy, type, *value};
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
