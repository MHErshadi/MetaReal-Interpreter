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

context_t context_copy_debug(const context_p context)
{
    context_t copy;

    copy.name = malloc(strlen(context->name) + 1);
    strcpy(copy.name, context->name);

    copy.parent = context->parent;
    copy.parent_pos = context->parent_pos;

    copy.fname = context->fname;

    if (copy.parent)
    {
        copy.parent = malloc(sizeof(context_t));
        *copy.parent = context_copy_debug(context->parent);
    }

    return copy;
}

void context_free(context_p context)
{
    free(context->name);
    table_delete(&context->table);
}

void context_free_debug(context_p context)
{
    free(context->name);

    if (context->parent)
        context_free_debug(context->parent);
}

void context_print(FILE* stream, const char* label, const context_p context, const char* end)
{
    if (context->name)
        fprintf(stream, "<%s %s at 0x%p>%s", label, context->name, context->name, end);
    else
        fprintf(stream, "<%s anonymous>%s", label, end);
}

value_p context_var_get(context_p context, unsigned char* type, const char* name, char* flag)
{
    value_p value = table_var_get(&context->table, type, name, flag);

    if (!value && context->parent)
        return context_var_get(context->parent, type, name, flag);

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
        strcpy(copy.vars[i].name, table->vars[i].name);

        copy.vars[i].type = table->vars[i].type;

        copy.vars[i].value = table->vars[i].value;
        value_copy(table->vars[i].value);
    }

    copy.alloc = table->alloc;
    copy.size = table->size;

    return copy;
}

void table_delete(table_p table)
{
    while (table->size)
    {
        value_free(table->vars[table->size].value);
        free(table->vars[--table->size].name);
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

        value_free(table->vars[i].value);
        free(table->vars[i].name);
    }

    table->size = j;
}

value_p table_var_get(table_p table, unsigned char* type, const char* name, char* flag)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if (VAR_CONST(table->vars[i].properties) && table->vars[i].value)
                *flag = 1;

            *type = table->vars[i].type;
            return table->vars[i].value;
        }

    return NULL;
}

var_p table_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if (VAR_CONST(table->vars[i].properties) && table->vars[i].value)
            {
                *flag = 0;
                return NULL;
            }

            if (table->vars[i].type != NONE_V && value && value->type != table->vars[i].type)
            {
                *flag = table->vars[i].type;
                return NULL;
            }

            value_free(table->vars[i].value);

            table->vars[i].properties |= properties;
            if (table->vars[i].type == NONE_V)
                table->vars[i].type = type;

            table->vars[i].value = value;

            return table->vars + i;
        }

    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    table->vars[table->size] = (var_t){properties, copy, type, value};

    return table->vars + table->size++;
}

var_p table_var_add(table_p table, const char* name)
{
    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    table->vars[table->size] = (var_t){0, copy, NONE_V, NULL};

    return table->vars + table->size++;
}
