/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/operation.h>
#include <complex.h>
#include <str.h>
#include <array/list.h>
#include <def.h>
#include <setting.h>
#include <lexer/token.h>
#include <string.h>
#include <stdlib.h>

#define IPROP_PTR(x) (x & 1)

#define APROP_PTR(x) (x | 1)

value_t ires_merge(ires_p ires, ires_t other);

ires_t interpret_node(node_p node, context_p context, char properties);
ires_t interpret_none(pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_tuple(list_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_dict(list_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_set(list_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_type(char node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_binary_operation(binary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_unary_operation(unary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_ternary_condition(ternary_condition_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_subscript(subscript_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_access(access_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_var_assign(var_assign_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_var_fixed_assign(var_fixed_assign_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_var_reassign(var_reassign_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_var_access(char* node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_func_def(func_def_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_func_call(func_call_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_class_def(class_def_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_struct_def(struct_def_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_dollar_func_call(dollar_func_call_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_if(if_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_switch(switch_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_for(for_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_foreach(foreach_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_loop(loop_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_do_while(do_while_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_while(while_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_try(try_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_import(char* node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_include(char* node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_return(return_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_continue(pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_break(pos_p poss, pos_p pose, context_p context, char properties);

ires_t interpret(node_p nodes, context_p context)
{
    ires_t ires;
    ires.value.type = NULL_V;

    node_p nodes_copy = nodes;

    while (nodes->type != NULL_N)
    {
        value_free(&ires.value);

        ires = interpret_node(nodes++, context, 0);
        if (ires.has_error)
        {
            node_p_free2(nodes);
            m_free(nodes_copy);
            return ires;
        }
    }

    m_free(nodes_copy);
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

value_t ires_merge(ires_p ires, ires_t other)
{
    if (other.has_error)
    {
        ires->error = other.error;
        ires->has_error = 1;
    }

    return other.value;
}

ires_t interpret_node(node_p node, context_p context, char properties)
{
    switch (node->type)
    {
    case NONE_N:
        return interpret_none(&node->poss, &node->pose, context, properties);
    case INT_N:
        return interpret_int(node->value.ptr, &node->poss, &node->pose, context, properties);
    case FLOAT_N:
        return interpret_float(node->value.ptr, &node->poss, &node->pose, context, properties);
    case COMPLEX_N:
        return interpret_complex(node->value.ptr, &node->poss, &node->pose, context, properties);
    case BOOL_N:
        return interpret_bool(node->value.chr, &node->poss, &node->pose, context, properties);
    case CHAR_N:
        return interpret_char(node->value.chr, &node->poss, &node->pose, context, properties);
    case STR_N:
        return interpret_str(node->value.ptr, &node->poss, &node->pose, context, properties);
    case LIST_N:
        return interpret_list(node->value.ptr, &node->poss, &node->pose, context, properties);
    case TUPLE_N:
        return interpret_tuple(node->value.ptr, &node->poss, &node->pose, context, properties);
    case TYPE_N:
        return interpret_type(node->value.chr, &node->poss, &node->pose, context, properties);
    case BINARY_OPERATION_N:
        return interpret_binary_operation(node->value.ptr, &node->poss, &node->pose, context, properties);
    case UNARY_OPERATION_N:
        return interpret_unary_operation(node->value.ptr, &node->poss, &node->pose, context, properties);
    case TERNARY_CONDITION_N:
        return interpret_ternary_condition(node->value.ptr, &node->poss, &node->pose, context, properties);
    case SUBSCRIPT_N:
        return interpret_subscript(node->value.ptr, &node->poss, &node->pose, context, properties);
    case VAR_ASSIGN_N:
        return interpret_var_assign(node->value.ptr, &node->poss, &node->pose, context, properties);
    case VAR_FIXED_ASSIGN_N:
        return interpret_var_fixed_assign(node->value.ptr, &node->poss, &node->pose, context, properties);
    case VAR_ACCESS_N:
        return interpret_var_access(node->value.ptr, &node->poss, &node->pose, context, properties);
    default:
        fprintf(stderr, "interpret_node function: invalid node type (#%u)\n", node->type);
        abort();
    }
}

ires_t interpret_none(pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(50);
        strcpy(detail, "<none> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    value_t value = value_set3(NONE_V, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(49);
        strcpy(detail, "<int> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    int_p ptr = int_set_str(node->value, node->size);
    value_t value = value_set1(INT_V, ptr, poss, pose, context);

    m_free(node->value);
    m_free(node);
    return ires_success(&value);
}

ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(51);
        strcpy(detail, "<float> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    float_p ptr = float_set_str(node->value, node->size, setting.float_prec_bit);
    value_t value = value_set1(FLOAT_V, ptr, poss, pose, context);

    m_free(node->value);
    m_free(node);
    return ires_success(&value);
}

ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(53);
        strcpy(detail, "<complex> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    complex_p ptr = complex_set_str(node->value, node->size, setting.complex_prec_bit);
    value_t value = value_set1(COMPLEX_V, ptr, poss, pose, context);

    m_free(node->value);
    m_free(node);
    return ires_success(&value);
}

ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(50);
        strcpy(detail, "<bool> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    value_t value = value_set2(BOOL_V, node, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(50);
        strcpy(detail, "<char> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    value_t value = value_set2(CHAR_V, node, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(49);
        strcpy(detail, "<str> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    str_p ptr = str_set_str(node->value, node->size);
    value_t value = value_set1(STR_V, ptr, poss, pose, context);

    m_free(node);
    return ires_success(&value);
}

ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(50);
        strcpy(detail, "<list> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    ires_t ires;
    ires.has_error = 0;

    if (!node)
    {
        list_p ptr = list_set(NULL, 0);
        ires.value = value_set1(LIST_V, ptr, poss, pose, context);

        return ires;
    }

    value_p elements = m_alloc(node->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_t element = ires_merge(&ires, interpret_node(&node->elements[i], context, 0));
        if (ires.has_error)
        {
            i++;
            while (node->size > i)
                node_free(&node->elements[--node->size]);
            m_free(node->elements);
            i--;

            while (i)
                value_free(&elements[--i]);
            m_free(elements);
            m_free(node);

            return ires;
        }

        elements[i] = element;
    }

    list_p ptr = list_set(elements, i);
    ires.value = value_set1(LIST_V, ptr, poss, pose, context);

    m_free(node->elements);
    m_free(node);
    return ires;
}

ires_t interpret_tuple(list_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(51);
        strcpy(detail, "<tuple> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    ires_t ires;
    ires.has_error = 0;

    value_p elements = m_alloc(node->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_t element = ires_merge(&ires, interpret_node(&node->elements[i], context, 0));
        if (ires.has_error)
        {
            i++;
            while (node->size > i)
                node_free(&node->elements[--node->size]);
            m_free(node->elements);
            i--;

            while (i)
                value_free(&elements[--i]);
            m_free(elements);
            m_free(node);

            return ires;
        }

        elements[i] = element;
    }

    tuple_p ptr = tuple_set(elements, i);
    ires.value = value_set1(TUPLE_V, ptr, poss, pose, context);

    m_free(node->elements);
    m_free(node);
    return ires;
}

ires_t interpret_dict(list_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_set(list_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_type(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(49);
        strcpy(detail, "<type> object can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    value_t value = value_set2(TYPE_V, node - OBJECT_TT + OBJECT_V, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_binary_operation(binary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(63);
        strcpy(detail, "Result of binary operation can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    ires_t ires;
    ires.has_error = 0;

    value_t left = ires_merge(&ires, interpret_node(&node->left, context, 0));
    if (ires.has_error)
    {
        node_free(&node->right);
        m_free(node);
        return ires;
    }

    value_t right = ires_merge(&ires, interpret_node(&node->right, context, 0));
    if (ires.has_error)
    {
        value_free(&left);
        m_free(node);
        return ires;
    }

    switch (node->operator)
    {
    case PLUS_T:
        ires = operate_add(&left, &right, poss, pose, context);
        break;
    case MINUS_T:
        ires = operate_subtract(&left, &right, poss, pose, context);
        break;
    case MULTIPLY_T:
        ires = operate_multiply(&left, &right, poss, pose, context);
        break;
    case DIVIDE_T:
        ires = operate_divide(&left, &right, poss, pose, context);
        break;
    case MODULO_T:
        ires = operate_modulo(&left, &right, poss, pose, context);
        break;
    case QUOTIENT_T:
        ires = operate_quotient(&left, &right, poss, pose, context);
        break;
    case POWER_T:
        ires = operate_power(&left, &right, poss, pose, context);
        break;
    case B_AND_T:
        ires = operate_b_and(&left, &right, poss, pose, context);
        break;
    case B_OR_T:
        ires = operate_b_or(&left, &right, poss, pose, context);
        break;
    case B_XOR_T:
        ires = operate_b_xor(&left, &right, poss, pose, context);
        break;
    case LSHIFT_T:
        ires = operate_lshift(&left, &right, poss, pose, context);
        break;
    case RSHIFT_T:
        ires = operate_rshift(&left, &right, poss, pose, context);
        break;
    case EQUAL_T:
        ires = operate_equal(&left, &right);
        break;
    case NEQUAL_T:
        ires = operate_nequal(&left, &right);
        break;
    case LESS_T:
        ires = operate_less(&left, &right, poss, pose, context);
        break;
    case GREATER_T:
        ires = operate_greater(&left, &right, poss, pose, context);
        break;
    case LESS_EQ_T:
        ires = operate_less_eq(&left, &right, poss, pose, context);
        break;
    case GREATER_EQ_T:
        ires = operate_greater_eq(&left, &right, poss, pose, context);
        break;
    case AND_T:
    case AND_TK:
        ires = operate_and(&left, &right);
        break;
    case OR_T:
    case OR_TK:
        ires = operate_or(&left, &right);
        break;
    case XOR_T:
    case XOR_TK:
        ires = operate_xor(&left, &right);
        break;
    case IN_TK:
        ires = operate_contain(&left, &right, poss, pose, context);
        break;
    case IS_TK:
        ires = operate_is(&left, &right);
        break;
    case ARE_TK:
        ires = operate_are(&left, &right, poss, pose, context);
        break;
    }

    if (ires.has_error)
    {
        m_free(node);
        return ires;
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    m_free(node);
    return ires;
}

ires_t interpret_unary_operation(unary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        char* detail = m_alloc(62);
        strcpy(detail, "Result of unary operation can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(&error);
    }

    ires_t ires;
    ires.has_error = 0;

    value_t operand = ires_merge(&ires, interpret_node(&node->operand, context, 0));
    if (ires.has_error)
    {
        m_free(node);
        return ires;
    }

    switch (node->operator)
    {
    case PLUS_T:
        ires = operate_positive(&operand);
        break;
    case MINUS_T:
        ires = operate_negate(&operand, poss, pose, context);
        break;
    case B_NOT_T:
        ires = operate_b_not(&operand, poss, pose, context);
        break;
    case NOT_T:
    case NOT_TK:
        ires = operate_not(&operand);
        break;
    }

    if (ires.has_error)
    {
        m_free(node);
        return ires;
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    m_free(node);
    return ires;
}

ires_t interpret_ternary_condition(ternary_condition_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.has_error = 0;

    value_t condition = ires_merge(&ires, interpret_node(&node->condition, context, 0));
    if (ires.has_error)
    {
        node_free(&node->left);
        node_free(&node->right);
        m_free(node);
        return ires;
    }

    value_t res;

    if (value_is_true(&condition))
    {
        node_free(&node->right);

        res = ires_merge(&ires, interpret_node(&node->left, context, properties));
        if (ires.has_error)
        {
            value_free(&condition);
            m_free(node);
            return ires;
        }
    }
    else
    {
        node_free(&node->left);

        res = ires_merge(&ires, interpret_node(&node->right, context, properties));
        if (ires.has_error)
        {
            value_free(&condition);
            m_free(node);
            return ires;
        }
    }

    ires.value = res;
    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    value_free(&condition);

    m_free(node);
    return ires;
}

ires_t interpret_subscript(subscript_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.has_error = 0;

    value_t value = ires_merge(&ires, interpret_node(&node->value, context, properties));
    if (ires.has_error)
    {
        node_free(&node->pos);
        m_free(node);
        return ires;
    }

    value_t pos = ires_merge(&ires, interpret_node(&node->pos, context, 0));
    if (ires.has_error)
    {
        if (!IPROP_PTR(properties))
            value_free(&value);
        m_free(node);
        return ires;
    }

    if (IPROP_PTR(properties))
    {
        ires = operate_subscript_ptr(value.value.ptr, &pos);

        if (ires.has_error)
        {
            m_free(node);
            return ires;
        }
    }
    else
    {
        ires = operate_subscript(&value, &pos);

        if (ires.has_error)
        {
            m_free(node);
            return ires;
        }

        ires.value.poss = *poss;
        ires.value.pose = *pose;
        ires.value.context = context;
    }

    m_free(node);
    return ires;
}

ires_t interpret_access(access_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_var_assign(var_assign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.has_error = 0;

    if (node->type)
        node->type -= OBJECT_TT - OBJECT_V;

    value_t value;
    if (node->value.type)
    {
        value = ires_merge(&ires, interpret_node(&node->value, context, 0));
        if (ires.has_error)
        {
            m_free(node->name);
            m_free(node);
            return ires;
        }

        if (node->type != NULL_V && node->type != value.type)
        {
            value_free(&value);
            m_free(node->name);
            m_free(node);

            char* detail = m_alloc(59 + value_label_lens[node->type] + value_label_lens[value.type]);
            sprintf(detail, "Type of variable and type of value do not match (<%s> and <%s>)", value_labels[node->type], value_labels[value.type]);

            runtime_t error = runtime_set(TYPE_E, detail, poss, pose, context);
            return ires_fail(&error);
        }
    }
    else
        value.type = NULL_V;

    context_p copy = context;

    if (PROP_GLOBAL(node->properties))
        while (copy->parent)
            copy = copy->parent;

    char res = 0;
    if (IPROP_PTR(properties))
    {
        value_p ptr = table_ptr_set(&copy->table, node->properties, node->name, node->type, &value, &res);
        if (res == -1)
        {
            value_free(&value);

            char* detail = m_alloc(26 + strlen(node->name));
            sprintf(detail, "'%s' is a constant variable", node->name);

            m_free(node->name);
            m_free(node);

            runtime_t error = runtime_set(CONST_E, detail, poss, pose, context);
            return ires_fail(&error);
        }
        if (res)
        {
            value_free(&value);

            char* detail = m_alloc(45 + strlen(node->name) + value_label_lens[res]);
            sprintf(detail, "'%s' is a type-specified variable (type is <%s>)", node->name, value_labels[res]);

            m_free(node->name);
            m_free(node);

            runtime_t error = runtime_set(TYPE_E, detail, poss, pose, context);
            return ires_fail(&error);
        }

        ires.value.value.ptr = ptr;
    }
    else
    {
        res = table_var_set(&copy->table, node->properties, node->name, node->type, &value);
        if (res == -1)
        {
            value_free(&value);

            char* detail = m_alloc(26 + strlen(node->name));
            sprintf(detail, "'%s' is a constant variable", node->name);

            m_free(node->name);
            m_free(node);

            runtime_t error = runtime_set(CONST_E, detail, poss, pose, context);
            return ires_fail(&error);
        }
        if (res)
        {
            value_free(&value);

            char* detail = m_alloc(45 + strlen(node->name) + value_label_lens[res]);
            sprintf(detail, "'%s' is a type-specified variable (type is <%s>)", node->name, value_labels[res]);

            m_free(node->name);
            m_free(node);

            runtime_t error = runtime_set(TYPE_E, detail, poss, pose, context);
            return ires_fail(&error);
        }

        ires.value = value_copy(&value);
        ires.value.poss = *poss;
        ires.value.pose = *pose;
        ires.value.context = context;
    }

    m_free(node);
    return ires;
}

ires_t interpret_var_fixed_assign(var_fixed_assign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.has_error = 0;

    value_t var = ires_merge(&ires, interpret_node(&node->var, context, APROP_PTR(properties)));
    if (ires.has_error)
    {
        m_free(node);
        return ires;
    }

    value_t res;
    if (PROP_POST(node->properties))
    {
        if (IPROP_PTR(properties))
            res.value.ptr = var.value.ptr;
        else
            res = value_copy(var.value.ptr);
    }
    else
        res.type = NULL_V;

    switch (node->operator)
    {
    case INCREMENT_T:
        ires = operate_increment(var.value.ptr, poss, pose, context);
        break;
    case DECREMENT_T:
        ires = operate_decrement(var.value.ptr, poss, pose, context);
        break;
    }

    if (ires.has_error)
    {
        value_free(&res);
        m_free(node);
        return ires;
    }

    if (res.type == NULL_V)
    {
        if (IPROP_PTR(properties))
            res.value.ptr = var.value.ptr;
        else
            res = value_copy(var.value.ptr);
    }

    ires.value = res;
    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    m_free(node);
    return ires;
}

ires_t interpret_var_reassign(var_reassign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_var_access(char* node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.has_error = 0;

    if (IPROP_PTR(properties))
    {
        char flag = 0;
        value_p value = context_ptr_get(context, node, &flag);

        if (flag)
        {
            char* detail = m_alloc(26 + strlen(node));
            sprintf(detail, "'%s' is a constant variable", node);

            m_free(node);

            runtime_t error = runtime_set(CONST_E, detail, poss, pose, context);
            return ires_fail(&error);
        }
        if (value == NULL)
        {
            char* detail = m_alloc(45 + strlen(node));
            sprintf(detail, "'%s' is not defined", node);

            m_free(node);

            runtime_t error = runtime_set(NOT_DEFINED_E, detail, poss, pose, context);
            return ires_fail(&error);
        }

        ires.value.value.ptr = value;
    }
    else
    {
        value_t value = context_var_get(context, node);
        if (value.type == NULL_V)
        {
            char* detail = m_alloc(45 + strlen(node));
            sprintf(detail, "'%s' is not defined", node);

            m_free(node);

            runtime_t error = runtime_set(NOT_DEFINED_E, detail, poss, pose, context);
            return ires_fail(&error);
        }

        ires.value = value_copy(&value);

        ires.value.poss = *poss;
        ires.value.pose = *pose;
        ires.value.context = context;
    }

    m_free(node);
    return ires;
}

ires_t interpret_func_def(func_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_func_call(func_call_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_class_def(class_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_struct_def(struct_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_dollar_func_call(dollar_func_call_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_if(if_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_switch(switch_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_for(for_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_foreach(foreach_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_loop(loop_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_do_while(do_while_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_while(while_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_try(try_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_import(char* node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_include(char* node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_return(return_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_continue(pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_break(pos_p poss, pos_p pose, context_p context, char properties)
{

}
