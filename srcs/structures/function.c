/*/
 * MetaReal version 1.0.0
/*/

#include <structures/function.h>
#include <stdlib.h>

func_p func_set(unsigned char type, context_p context, body_p body)
{
    func_p func = malloc(sizeof(func_t));

    func->type = type;
    func->context = *context;
    func->body = *body;

    return func;
}

func_p func_copy(const func_p func)
{
    func_p copy = malloc(sizeof(func_t));

    copy->type = func->type;

    copy->context = context_copy(&func->context);

    copy->body.nodes = node_p_copy(func->body.nodes, func->body.size);
    copy->body.size = func->body.size;

    return copy;
}

void func_free(func_p func)
{
    free(func->context.name);
    table_delete(&func->context.table);

    node_p_free1(func->body.nodes, func->body.size);
    free(func);
}
