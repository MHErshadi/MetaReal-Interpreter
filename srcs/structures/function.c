/*/
 * MetaReal version 1.0.0
/*/

#include <structures/function.h>
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

func_p func_copy(const func_p func)
{
    func_p copy = malloc(sizeof(func_t));

    copy->type = func->type;

    copy->min_size = func->min_size;
    copy->max_size = func->max_size;


    copy->size = func->size;

    copy->context = context_copy(&func->context);

    copy->body.nodes = node_p_copy(func->body.nodes, func->body.size);
    copy->body.size = func->body.size;

    return copy;
}

void func_free(func_p func)
{
    free(func->context.name);
    table_delete(&func->context.table);

    arg_value_free(func->args, func->size);

    node_p_free1(func->body.nodes, func->body.size);

    free(func);
}

void func_print(FILE* stream, const func_p func, const char* end)
{
    if (func->context.name)
        fprintf(stream, "<function %s at 0x%p>%s", func->context.name, func->context.name, end);
    else
        fprintf(stream, "<function <anonymous> at 0x%p>%s", func->context.name, end);
}

void arg_value_free(arg_value_p args, unsigned long long size)
{
    if (!size)
        return;

    while (size)
        value_free(&args[--size].value);
    free(args);
}

void func_table_free(table_p table, unsigned long long arg_size)
{
    unsigned long long i, j;
    for (i = 0; i < arg_size; i++)
    {
        value_delete(&table->vars[i].value);
        table->vars[i].value.type = NULL_V;
    }

    for (j = arg_size; i < table->size; i++)
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
