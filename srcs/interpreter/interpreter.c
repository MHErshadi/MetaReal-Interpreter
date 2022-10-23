/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <int.h>
#include <float.h>
#include <complex.h>
#include <str.h>
#include <stdlib.h>
#include <setting.h>

ires_t ires_success(value_p value);
ires_t ires_fail(runtime_p error);

value_t ires_merge(ires_p ires, ires_p other);

ires_t interpret_node(node_p node, context_p context);
ires_t interpret_none(pos_p poss, pos_p pose, context_p context);
ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context);

ires_t interpret(node_p nodes, context_p context)
{
    ires_t ires;
    ires.value.type = NULL_V;

    node_p nodes_copy = nodes;

    while (nodes->type != NULL_N)
    {
        value_free(&ires.value);

        ires = interpret_node(nodes++, context);
        if (ires.has_error)
        {
            node_p_free2(nodes);
            free(nodes_copy);
            return ires;
        }
    }

    free(nodes_copy);
    return ires;
}

ires_t ires_success(value_p value)
{
    ires_t ires;

    ires.value = *value;
    ires.has_error = 0;

    return ires;
}

ires_t ires_fail(runtime_p error)
{
    ires_t ires;

    ires.error = *error;
    ires.has_error = 1;

    return ires;
}

value_t ires_merge(ires_p ires, ires_p other)
{
    if (other->has_error)
    {
        ires->error = other->error;
        ires->has_error = 1;
    }

    return other->value;
}

ires_t interpret_node(node_p node, context_p context)
{
    switch (node->type)
    {
    case NONE_N:
        return interpret_none(&node->poss, &node->pose, context);
    case INT_N:
        return interpret_int(node->value.ptr, &node->poss, &node->pose, context);
    case FLOAT_N:
        return interpret_float(node->value.ptr, &node->poss, &node->pose, context);
    case COMPLEX_N:
        return interpret_complex(node->value.ptr, &node->poss, &node->pose, context);
    case BOOL_N:
        return interpret_bool(node->value.chr, &node->poss, &node->pose, context);
    case CHAR_N:
        return interpret_char(node->value.chr, &node->poss, &node->pose, context);
    case STR_N:
        return interpret_str(node->value.ptr, &node->poss, &node->pose, context);
    default:
        fprintf(stderr, "interpret_node function: invalid node type (#%u)\n", node->type);
        abort();
    }
}

ires_t interpret_none(pos_p poss, pos_p pose, context_p context)
{
    value_t value = value_set3(NONE_V, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context)
{
    int_p ptr = int_set_str(node->value, node->size);
    value_t value = value_set1(INT_V, ptr, poss, pose, context);

    free(node->value);
    free(node);
    return ires_success(&value);
}

ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context)
{
    float_p ptr = float_set_str(setting.float_prec_bit, node->value, node->size);
    value_t value = value_set1(FLOAT_V, ptr, poss, pose, context);

    free(node->value);
    free(node);
    return ires_success(&value);
}

ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context)
{
    complex_p ptr = complex_set_str(setting.complex_prec_bit, node->value, node->size);
    value_t value = value_set1(COMPLEX_V, ptr, poss, pose, context);

    free(node->value);
    free(node);
    return ires_success(&value);
}

ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context)
{
    value_t value = value_set2(BOOL_V, node, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context)
{
    value_t value = value_set2(CHAR_V, node, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context)
{
    str_p ptr = str_set_str(node->value, node->size);
    value_t value = value_set1(STR_V, ptr, poss, pose, context);

    free(node);
    return ires_success(&value);
}
