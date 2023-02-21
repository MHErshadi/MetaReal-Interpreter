/*/
 * MetaReal version 1.0.0
/*/

#include <struct/func.h>
#include <stdlib.h>

func_p func_set(unsigned char type, unsigned long long min_size, unsigned long long max_size,
    arg_value_p args, unsigned long long size,
    context_p context, body_p body)
{
    func_p func = malloc(sizeof(func_t));

    func->type = type;
    func->min_size = min_size;
    func->max_size = max_size;
    func->args = args;
    func->size = size;
    func->context = *context;
    func->body = *body;

    return func;
}

void func_free(func_p func)
{
    free(func->context.name);
    table_delete(&func->context.table);

    arg_value_free(func->args, func->size);

    node_p_free1(func->body.nodes, func->body.size);

    free(func);
}

void arg_value_free(arg_value_p args, unsigned long long size)
{
    if (!size)
        return;

    while (size)
        value_free(args[--size].value);
    free(args);
}

void func_table_free(table_p table, unsigned long long arg_size)
{
    unsigned long long i, j;
    for (i = arg_size, j = arg_size; i < table->size; i++)
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

void func_table_free_args(table_p table, unsigned long long arg_size)
{
    unsigned long long i;
    for (i = 0; i < arg_size; i++)
    {
        value_free(table->vars[i].value);
        table->vars[i].value = NULL;
    }
}
