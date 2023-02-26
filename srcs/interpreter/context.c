/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/context.h>
#include <int.h>
#include <struct/bi_func.h>
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

    free(context);
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

value_p context_var_get(context_p context, const char* name)
{
    value_p value = table_var_get(&context->table, name);

    if (!value && context->parent)
        return context_var_get(context->parent, name);

    return value;
}

var_p context_ptr_get(context_p context, const char* name, char* flag)
{
    var_p ptr = table_ptr_get(&context->table, name, flag);

    if (!ptr && !*flag && context->parent)
        return context_ptr_get(context->parent, name, flag);

    return ptr;
}

table_t table_set(unsigned long long alloc)
{
    table_t table;

    table.vars = malloc(alloc * sizeof(var_t));
    table.alloc = alloc;
    table.size = 0;

    return table;
}

table_t table_set_def()
{
    table_t table;

    table.vars = malloc(TABLE_SIZE * sizeof(var_t));
    table.alloc = TABLE_SIZE;
    table.size = 7;

    *table.vars = (var_t){
        DEF_VAR, "null",
        INT_V, value_set1(INT_V, int_set_ull(0))
    };
    table.vars[1] = (var_t){
        DEF_VAR, "print",
        BI_FUNC_V, value_set2(BI_FUNC_V, PRINT_BI)
    };
    table.vars[2] = (var_t){
        DEF_VAR, "input",
        BI_FUNC_V, value_set2(BI_FUNC_V, INPUT_BI)
    };
    table.vars[3] = (var_t){
        DEF_VAR, "typeof",
        BI_FUNC_V, value_set2(BI_FUNC_V, TYPEOF_BI)
    };
    table.vars[4] = (var_t){
        DEF_VAR, "exit",
        BI_FUNC_V, value_set2(BI_FUNC_V, EXIT_BI)
    };
    table.vars[5] = (var_t){
        DEF_VAR, "len",
        BI_FUNC_V, value_set2(BI_FUNC_V, LEN_BI)
    };
    table.vars[6] = (var_t){
        DEF_VAR, "ptr",
        BI_FUNC_V, value_set2(BI_FUNC_V, PTR_BI)
    };

    return table;
}

void table_delete(table_p table)
{
    while (table->size)
    {
        value_free(table->vars[--table->size].value);

        if (!VAR_DEFAULT(table->vars[table->size].properties))
            free(table->vars[table->size].name);
    }

    if (table->alloc)
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

value_p table_var_get(table_p table, const char* name)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
            return table->vars[i].value;

    return NULL;
}

var_p table_ptr_get(table_p table, const char* name, char* flag)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if (VAR_CONST(table->vars[i].properties) && table->vars[i].value)
            {
                *flag = 1;
                return NULL;
            }

            return table->vars + i;
        }

    return NULL;
}

char table_var_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value)
{
    unsigned long long i;
    for (i = 0; i < table->size; i++)
        if (!strcmp(name, table->vars[i].name))
        {
            if (VAR_CONST(table->vars[i].properties) && table->vars[i].value)
                return -1;

            if (table->vars[i].type != NONE_V && value && value->type != table->vars[i].type)
                return table->vars[i].type;

            value_free(table->vars[i].value);

            table->vars[i].properties |= properties;
            if (table->vars[i].type == NONE_V)
                table->vars[i].type = type;

            table->vars[i].value = value;
            return 0;
        }

    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    table->vars[table->size++] = (var_t){properties, copy, type, value};
    return 0;
}

var_p table_ptr_set(table_p table, unsigned char properties, const char* name, unsigned char type, value_p value, char* flag)
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

var_p table_ptr_add(table_p table, const char* name)
{
    if (table->alloc == table->size)
        table->vars = realloc(table->vars, (table->alloc *= 2) * sizeof(var_t));

    char* copy = malloc(strlen(name) + 1);
    strcpy(copy, name);

    table->vars[table->size] = (var_t){0, copy, NONE_V, NULL};
    return table->vars + table->size++;
}
