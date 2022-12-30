/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/operation.h>
#include <interpreter/dollar_func.h>
#include <debugger/runtime_error.h>
#include <complex.h>
#include <str.h>
#include <array/list.h>
#include <stdlib.h>
#include <setting.h>
#include <lexer/token.h>
#include <string.h>
#include <stdlib.h>
#include <def.h>

#define IPROP_SET(ptr, assign, in_loop, should_copy) ((ptr) | (assign) << 1 | (in_loop) << 2 | (should_copy) << 3)
#define IPROP_MASK 0b100

#define IPROP_PTR(x) ((x) & 1)
#define IPROP_ASSIGN(x) ((x) >> 1 & 1)
#define IPROP_IN_LOOP(x) ((x) >> 2 & 1)
#define IPROP_SHOULD_COPY(x) ((x) >> 3 & 1)

value_t ires_merge(ires_p ires, ires_t other);

ires_t interpret_node(node_p node, context_p context, char properties);
ires_t interpret_body(body_p body, context_p context, char properties);

ires_t interpret_none(pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_tuple(tuple_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_dict(dict_np node, pos_p poss, pos_p pose, context_p context, char properties);
ires_t interpret_set(set_np node, pos_p poss, pos_p pose, context_p context, char properties);
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
        if (IRES_HAS_ERROR(ires.response))
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
    ires.response = 0;
    ires.response = 0;

    return ires;
}

ires_t ires_fail(runtime_t error)
{
    ires_t ires;

    ires.error = error;
    ires.response = 1;

    return ires;
}

value_t ires_merge(ires_p ires, ires_t other)
{
    if (IRES_HAS_ERROR(other.response))
        ires->error = other.error;

    ires->response |= other.response;

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
    case VAR_REASSIGN_N:
        return interpret_var_reassign(node->value.ptr, &node->poss, &node->pose, context, properties);
    case VAR_ACCESS_N:
        return interpret_var_access(node->value.ptr, &node->poss, &node->pose, context, properties);
    case FUNC_DEF_N:
        return interpret_func_def(node->value.ptr, &node->poss, &node->pose, context, properties);
    case DOLLAR_FUNC_CALL_N:
        return interpret_dollar_func_call(node->value.ptr, &node->poss, &node->pose, context, properties);
    case IF_N:
        return interpret_if(node->value.ptr, &node->poss, &node->pose, context, properties);
    case SWITCH_N:
        return interpret_switch(node->value.ptr, &node->poss, &node->pose, context, properties);
    case FOR_N:
        return interpret_for(node->value.ptr, &node->poss, &node->pose, context, properties);
    case FOREACH_N:
        return interpret_foreach(node->value.ptr, &node->poss, &node->pose, context, properties);
    case LOOP_N:
        return interpret_loop(node->value.ptr, &node->poss, &node->pose, context, properties);
    case DO_WHILE_N:
        return interpret_do_while(node->value.ptr, &node->poss, &node->pose, context, properties);
    case WHILE_N:
        return interpret_while(node->value.ptr, &node->poss, &node->pose, context, properties);
    case TRY_N:
        return interpret_try(node->value.ptr, &node->poss, &node->pose, context, properties);
    case CONTINUE_N:
        return interpret_continue(&node->poss, &node->pose, context, properties);
    case BREAK_N:
        return interpret_break(&node->poss, &node->pose, context, properties);
    default:
        fprintf(stderr, "interpret_node function: invalid node type (#%u)\n", node->type);
        abort();
    }
}

ires_t interpret_body(body_p body, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    unsigned long long i = 0;
    while (i < body->size)
    {
        value_free(&ires.value);

        ires.value = ires_merge(&ires, interpret_node(&body->nodes[i++], context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            while (body->size > i)
                node_free(body->nodes + --body->size);
            free(body->nodes);
            return ires;
        }

        if (IRES_LOOP_CONTINUE(ires.response) || IRES_LOOP_BREAK(ires.response))
            break;
    }

    if (!IPROP_IN_LOOP(properties))
        free(body->nodes);

    return ires;
}

ires_t interpret_none(pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(NONE_V, poss, pose, context));

    value_t value = value_set3(NONE_V, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            int_n_free(node);

        return ires_fail(invalid_access_object(INT_V, poss, pose, context));
    }

    int_p ptr = int_set_str(node->value, node->size);
    value_t value = value_set1(INT_V, ptr, poss, pose, context);

    if (!IPROP_IN_LOOP(properties))
        int_n_free(node);

    return ires_success(&value);
}

ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            float_n_free(node);

        return ires_fail(invalid_access_object(FLOAT_V, poss, pose, context));
    }

    float_p ptr = float_set_str(node->value, node->size, setting.float_prec_bit);
    value_t value = value_set1(FLOAT_V, ptr, poss, pose, context);

    if (!IPROP_IN_LOOP(properties))
        float_n_free(node);

    return ires_success(&value);
}

ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            complex_n_free(node);

        return ires_fail(invalid_access_object(COMPLEX_V, poss, pose, context));
    }

    complex_p ptr = complex_set_str(node->value, node->size, setting.complex_prec_bit);
    value_t value = value_set1(COMPLEX_V, ptr, poss, pose, context);

    if (!IPROP_IN_LOOP(properties))
        complex_n_free(node);

    return ires_success(&value);
}

ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(BOOL_V, poss, pose, context));

    value_t value = value_set2(BOOL_V, node, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(CHAR_V, poss, pose, context));

    value_t value = value_set2(CHAR_V, node, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            str_n_free(node);

        return ires_fail(invalid_access_object(STR_V, poss, pose, context));
    }

    str_p ptr = str_set_str(node->value, node->size);
    value_t value = value_set1(STR_V, ptr, poss, pose, context);

    if (!IPROP_IN_LOOP(properties))
        str_n_free(node);

    return ires_success(&value);
}

ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (node && !IPROP_IN_LOOP(properties))
            list_n_free(node);

        return ires_fail(invalid_access_object(LIST_V, poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    if (!node)
    {
        list_p ptr = list_set(NULL, 0);
        ires.value = value_set1(LIST_V, ptr, poss, pose, context);

        return ires;
    }

    value_p elements = malloc(node->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_t element = ires_merge(&ires, interpret_node(&node->elements[i], context, properties | IPROP_SET(0, 0, 0, 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            while (i)
                value_free(elements + --i);
            free(elements);

            if (!IPROP_IN_LOOP(properties))
            {
                i++;
                while (node->size > i)
                    node_free(node->elements + --node->size);
                free(node->elements);
                i--;

                free(node);
            }

            return ires;
        }

        elements[i] = element;
    }

    list_p ptr = list_set(elements, i);
    ires.value = value_set1(LIST_V, ptr, poss, pose, context);

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->elements);
        free(node);
    }

    return ires;
}

ires_t interpret_tuple(tuple_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            tuple_n_free(node);

        return ires_fail(invalid_access_object(TUPLE_V, poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p elements = malloc(node->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_t element = ires_merge(&ires, interpret_node(&node->elements[i], context, properties | IPROP_SET(0, 0, 0, 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            while (i)
                value_free(elements + --i);
            free(elements);

            if (!IPROP_IN_LOOP(properties))
            {
                i++;
                while (node->size > i)
                    node_free(node->elements + --node->size);
                free(node->elements);
                i--;

                free(node);
            }

            return ires;
        }

        elements[i] = element;
    }

    tuple_p ptr = tuple_set(elements, i);
    ires.value = value_set1(TUPLE_V, ptr, poss, pose, context);

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->elements);
        free(node);
    }

    return ires;
}

ires_t interpret_dict(dict_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_set(set_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_type(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(TYPE_V, poss, pose, context));

    value_t value = value_set2(TYPE_V, node - OBJECT_TT + OBJECT_V, poss, pose, context);

    return ires_success(&value);
}

ires_t interpret_binary_operation(binary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            binary_operation_n_free(node);

        return ires_fail(invalid_access_statement("binary operation", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    char should_copy = node->operator >= PLUS_T && node->operator <= B_NOT_T;

    value_t left = ires_merge(&ires, interpret_node(&node->left, context, properties | IPROP_SET(0, 0, 0, should_copy)));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_free(&node->right);
            free(node);
        }
        return ires;
    }

    value_t right;

    switch (node->operator)
    {
    case AND_T:
    case AND_TK:
        ires.value.value.chr = value_is_true(&left);
        if (ires.value.value.chr)
        {
            right = ires_merge(&ires, interpret_node(&node->right, context, properties));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(&left);

                if (!IPROP_IN_LOOP(properties))
                    free(node);

                return ires;
            }

            ires.value.value.chr &= value_is_true(&right);

            value_free(&right);
        }

        value_free(&left);

        ires.value.type = BOOL_V;
        goto ret;
    case OR_T:
    case OR_TK:
        ires.value.value.chr = value_is_true(&left);
        if (!ires.value.value.chr)
        {
            right = ires_merge(&ires, interpret_node(&node->right, context, properties));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(&left);

                if (!IPROP_IN_LOOP(properties))
                    free(node);

                return ires;
            }

            ires.value.value.chr |= value_is_true(&right);

            value_free(&right);
        }

        value_free(&left);

        ires.value.type = BOOL_V;
        goto ret;
    }

    right = ires_merge(&ires, interpret_node(&node->right, context, properties | IPROP_SET(0, 0, 0, should_copy)));
    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(&left);

        if (!IPROP_IN_LOOP(properties))
            free(node);

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

    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

ret:
    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
        free(node);

    return ires;
}

ires_t interpret_unary_operation(unary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            unary_operation_n_free(node);

        return ires_fail(invalid_access_statement("unary operation", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_t operand = ires_merge(&ires, interpret_node(&node->operand, context, properties | IPROP_SET(0, 0, 0, 1)));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
            free(node);
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

    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
        free(node);

    return ires;
}

ires_t interpret_ternary_condition(ternary_condition_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_t condition = ires_merge(&ires, interpret_node(&node->condition, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_free(&node->left);
            node_free(&node->right);
            free(node);
        }
        return ires;
    }

    value_t res;

    if (value_is_true(&condition))
    {
        if (!IPROP_IN_LOOP(properties))
            node_free(&node->right);

        res = ires_merge(&ires, interpret_node(&node->left, context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(&condition);

            if (!IPROP_IN_LOOP(properties))
                free(node);
            return ires;
        }
    }
    else
    {
        if (!IPROP_IN_LOOP(properties))
            node_free(&node->left);

        res = ires_merge(&ires, interpret_node(&node->right, context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(&condition);

            if (!IPROP_IN_LOOP(properties))
                free(node);
            return ires;
        }
    }

    ires.value = res;
    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    value_free(&condition);

    if (!IPROP_IN_LOOP(properties))
        free(node);

    return ires;
}

ires_t interpret_subscript(subscript_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_t value = ires_merge(&ires, interpret_node(&node->value, context, properties));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_free(&node->pos);
            free(node);
        }
        return ires;
    }

    value_t pos = ires_merge(&ires, interpret_node(&node->pos, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_PTR(properties))
            value_free(&value);
        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

    if (IPROP_PTR(properties))
    {
        ires = operate_subscript_ptr(value.value.ptr, &pos, &value.poss, &value.pose);

        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
                free(node);
            return ires;
        }
    }
    else
    {
        ires = operate_subscript(&value, &pos);

        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
                free(node);
            return ires;
        }
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
        free(node);

    return ires;
}

ires_t interpret_access(access_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_var_assign(var_assign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    if (node->type)
        node->type -= OBJECT_TT - OBJECT_V;

    value_t value;
    if (node->value.type)
    {
        value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                free(node->name);
                free(node);
            }
            return ires;
        }

        if (node->type != NULL_V && node->type != value.type)
        {
            value_free(&value);

            if (!IPROP_IN_LOOP(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(type_match(node->type, value.type, poss, pose, context));
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
        value_p ptr = table_ptr_set(&copy->table, &ires.value.type, node->properties, node->name, node->type, &value, &res);
        if (res == -1)
        {
            value_free(&value);

            runtime_t error = const_variable(node->name, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
        if (res)
        {
            value_free(&value);

            runtime_t error = type_specified_variable(node->name, res, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }

        ires.value.value.ptr = ptr;
    }
    else
    {
        res = table_var_set(&copy->table, node->properties, node->name, node->type, &value);
        if (res == -1)
        {
            value_free(&value);

            runtime_t error = const_variable(node->name, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
        if (res)
        {
            value_free(&value);

            runtime_t error = type_specified_variable(node->name, res, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }

        ires.value = value_copy(&value);
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->name);
        free(node);
    }

    return ires;
}

ires_t interpret_var_fixed_assign(var_fixed_assign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_t var = ires_merge(&ires, interpret_node(&node->var, context, properties | IPROP_SET(1, 0, 0, 0)));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

    value_t res;
    if (node->properties)
    {
        if (IPROP_PTR(properties))
        {
            res.type = var.type;
            res.value.ptr = var.value.ptr;
        }
        else
        {
            if (IRES_CHAR_PTR(ires.response))
            {
                res.type = CHAR_V;
                res.value.chr = *(char*)var.value.ptr;
            }
            else
                res = value_copy(var.value.ptr);
        }
    }

    switch (node->operator)
    {
    case INCREMENT_T:
        if (IRES_CHAR_PTR(ires.response))
            (*(char*)var.value.ptr)++;
        else
            ires = operate_increment(var.value.ptr, poss, pose, context);
        break;
    case DECREMENT_T:
        if (IRES_CHAR_PTR(ires.response))
            (*(char*)var.value.ptr)--;
        else
            ires = operate_decrement(var.value.ptr, poss, pose, context);
        break;
    }

    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(&res);

        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

    if (!node->properties)
    {
        if (IPROP_PTR(properties))
        {
            res.type = var.type;
            res.value.ptr = var.value.ptr;
        }
        else
        {
            if (IRES_CHAR_PTR(ires.response))
            {
                res.type = CHAR_V;
                res.value.chr = *(char*)var.value.ptr;
            }
            else
                res = value_copy(var.value.ptr);
        }
    }

    ires.value = res;
    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
        free(node);

    if (!IPROP_PTR(properties))
        ires.response = 0;

    return ires;
}

ires_t interpret_var_reassign(var_reassign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_t var = ires_merge(&ires, interpret_node(&node->var, context, properties | IPROP_SET(1, node->operator == ASSIGN_T, 0, 0)));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_free(&node->value);
            free(node);
        }
        return ires;
    }

    ires.value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

    value_t copy;
    if (!IRES_CHAR_PTR(ires.response))
        copy = value_copy(var.value.ptr);

    switch (node->operator)
    {
    case ASSIGN_T:
        if (var.type != NULL_V && var.type != ires.value.type)
        {
            if (!IPROP_IN_LOOP(properties))
                free(node);

            return ires_fail(assign_type_specified_variable(ires.value.type, var.type, poss, pose, context));
        }

        if (IRES_CHAR_PTR(ires.response))
            *(char*)var.value.ptr = ires.value.value.chr;
        else
        {
            value_free(var.value.ptr);
            *(value_p)var.value.ptr = ires.value;
        }

        break;
    case PLUS_EQ_T:
        ires = operate_add(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case MINUS_EQ_T:
        ires = operate_subtract(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case MULTIPLY_EQ_T:
        ires = operate_multiply(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case DIVIDE_EQ_T:
        ires = operate_divide(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case MODULO_EQ_T:
        ires = operate_modulo(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case QUOTIENT_EQ_T:
        ires = operate_quotient(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case POWER_EQ_T:
        ires = operate_power(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case B_AND_EQ_T:
        ires = operate_b_and(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case B_OR_EQ_T:
        ires = operate_b_or(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case B_XOR_EQ_T:
        ires = operate_b_xor(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case LSHIFT_EQ_T:
        ires = operate_lshift(var.value.ptr, &ires.value, poss, pose, context);
        break;
    case RSHIFT_EQ_T:
        ires = operate_rshift(var.value.ptr, &ires.value, poss, pose, context);
        break;
    }

    if (IRES_HAS_ERROR(ires.response))
    {
        *(value_p)var.value.ptr = copy;

        if (!IPROP_IN_LOOP(properties))
            free(node);
        return ires;
    }

    if (node->operator != ASSIGN_T)
    {
        if (var.type != NULL_V && var.type != ires.value.type)
        {
            *(value_p)var.value.ptr = copy;

            if (!IPROP_IN_LOOP(properties))
                free(node);

            return ires_fail(assign_type_specified_variable(ires.value.type, var.type, poss, pose, context));
        }

        *(value_p)var.value.ptr = ires.value;
    }

    if (IRES_CHAR_PTR(ires.response))
        ires.value.value.chr = *(char*)var.value.ptr;
    else
    {
        value_free(&copy);

        ires.value = value_copy(var.value.ptr);
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
        free(node);

    if (!IPROP_PTR(properties))
        ires.response = 0;

    return ires;
}

ires_t interpret_var_access(char* node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    if (IPROP_PTR(properties))
    {
        char flag = 0;
        value_p value = context_ptr_get(context, &ires.value.type, node, &flag);

        if (flag)
        {
            runtime_t error = const_variable(node, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
                free(node);

            return ires_fail(error);
        }
        if (!value)
        {
            if (IPROP_ASSIGN(properties))
            {
                ires.value.type = NULL_V;
                ires.value.value.ptr = table_ptr_add(&context->table, node);
                return ires;
            }
            else
            {
                runtime_t error = not_defined(node, poss, pose, context);

                if (!IPROP_IN_LOOP(properties))
                    free(node);

                return ires_fail(error);
            }
        }

        ires.value.value.ptr = value;
    }
    else
    {
        ires.value = context_var_get(context, node);
        if (ires.value.type == NULL_V)
        {
            runtime_t error = not_defined(node, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
                free(node);

            return ires_fail(error);
        }

        if (IPROP_SHOULD_COPY(properties))
            ires.value = value_copy(&ires.value);
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
        free(node);

    return ires;
}

ires_t interpret_func_def(func_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    /*
    // ill defined

    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            arg_p_free(node->args, node->size);
            free(node->name);
            free(node);
        }

        char* detail = malloc(66);
        strcpy(detail, "Result of function definition can not be accessed like a variable");

        runtime_t error = runtime_set(ACCESS_E, detail, poss, pose, context);
        return ires_fail(error);
    }

    ires_t ires;
    ires.has_error = 0;

    if (node->type)
        node->type -= OBJECT_TT - OBJECT_V;

    table_t table = table_set(FUNC_CONTEXT_SIZE + node->size);

    for (; table.size < node->size; table.size++)
    {
        table.vars[table.size].properties = 0;

        if (node->args[table.size].type)
            node->args[table.size].type -= OBJECT_TT - OBJECT_V;

        table.vars[table.size].type = node->args[table.size].type;

        if (node->args[table.size].value.type)
        {
            table.vars[table.size].value = ires_merge(&ires, interpret_node(&node->args[table.size].value, context, properties & IPROP_MASK));
            if (ires.has_error)
            {
                table_free(&table);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);

                    while (node->size > table.size + 1)
                    {
                        node_free(&node->args[node->size].value);
                        free(node->args[node->size--].name);
                    }
                    free(node->args);

                    free(node->name);
                    free(node);
                }

                return ires;
            }

            if (table.vars[table.size].type && table.vars[table.size].type != table.vars[table.size].value.type)
            {
                table_free(&table);

                char* detail = malloc(59 + value_label_lens[table.vars[table.size].type] + value_label_lens[table.vars[table.size].value.type]);
                sprintf(detail, "Type of variable and type of value do not match (<%s> and <%s>)",
                    value_labels[table.vars[table.size].type], value_labels[table.vars[table.size].value.type]);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);

                    while (node->size > table.size + 1)
                    {
                        node_free(&node->args[node->size].value);
                        free(node->args[node->size--].name);
                    }
                    free(node->args);

                    free(node->name);
                    free(node);
                }

                runtime_t error = runtime_set(TYPE_E, detail, &table.vars[table.size].value.poss, &table.vars[table.size].value.pose, table.vars[table.size].value.context);
                return ires_fail(error);
            }
        }
        else
            table.vars[table.size].value.type = NULL_V;

        table.vars[table.size].name = malloc(strlen(node->args[table.size].name) + 1);
        strcpy(table.vars[table.size].name, node->args[table.size].name);

        if (!IPROP_IN_LOOP(properties))
            free(node->args[table.size].name);
    }

    context_p copy = context;

    if (PROP_GLOBAL(node->properties))
        while (copy->parent)
            copy = copy->parent;

    char* name = malloc(strlen(node->name) + 1);
    strcpy(name, node->name);

    context_t func_context = context_set1(name, copy, poss, &table, copy->fname);

    func_p func = func_set(node->type, &func_context, &node->body);
    ires.value = value_set1(FUNC_V, func, poss, pose, context);

    char res = table_var_set(&copy->table, node->properties, node->name, node->type, &ires.value);
    if (res == -1)
    {
        func_free(func);

        char* detail = malloc(26 + strlen(node->name));
        sprintf(detail, "'%s' is a constant variable", node->name);

        if (!IPROP_IN_LOOP(properties))
        {
            free(node->args);
            free(node->name);
            free(node);
        }

        runtime_t error = runtime_set(CONST_E, detail, poss, pose, context);
        return ires_fail(error);
    }
    if (res)
    {
        func_free(func);

        char* detail = malloc(45 + strlen(node->name) + value_label_lens[(unsigned)res]);
        sprintf(detail, "'%s' is a type-specified variable (type is <%s>)",
            node->name, value_labels[(unsigned)res]);

        if (!IPROP_IN_LOOP(properties))
        {
            free(node->args);
            free(node->name);
            free(node);
        }

        runtime_t error = runtime_set(TYPE_E, detail, poss, pose, context);
        return ires_fail(error);
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->args);
        free(node->name);
        free(node);
    }

    return ires;
    */
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
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            dollar_func_call_n_free(node);

        return ires_fail(invalid_access_statement("dollar-function call", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    
    value_p args = malloc(node->size * sizeof(value_t));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_t arg = ires_merge(&ires, interpret_node(&node->args[i], context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            while (i)
                value_free(args + --i);
            free(args);

            if (!IPROP_IN_LOOP(properties))
            {
                i++;
                while (node->size > i)
                    node_free(node->args + --node->size);
                free(node->args);
                i--;

                free(node->name);
                free(node);
            }

            return ires;
        }

        args[i] = arg;
    }

    ires.value = ires_merge(&ires, handle_dollar_func(node->name, args, i, poss, pose, context));

    while (i)
        value_free(args + --i);
    free(args);

    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            free(node->args);
            free(node->name);
            free(node);
        }
        return ires;
    }

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->args);
        free(node->name);
        free(node);
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    return ires;
}

ires_t interpret_if(if_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            if_n_free(node);

        return ires_fail(invalid_access_statement("if statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    unsigned long long i = 0;
    while (i < node->size)
    {
        value_t condition = ires_merge(&ires, interpret_node(&node->cases[i++].condition, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->ebody.nodes, node->ebody.size);

                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->cases[node->size].body.nodes, node->cases[node->size].body.size);
                    node_free(&node->cases[node->size].condition);
                }
                free(node->cases);

                free(node);
            }

            return ires;
        }

        if (value_is_true(&condition))
        {
            value_free(&condition);

            ires.value = ires_merge(&ires, interpret_body(&node->cases[i - 1].body, context, properties & IPROP_MASK));

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->ebody.nodes, node->ebody.size);

                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->cases[node->size].body.nodes, node->cases[node->size].body.size);
                    node_free(&node->cases[node->size].condition);
                }
                free(node->cases);

                free(node);
            }

            if (IRES_HAS_ERROR(ires.response))
                return ires;

            ires.value.poss = *poss;
            ires.value.pose = *pose;
            ires.value.context = context;

            return ires;
        }

        value_free(&condition);

        if (!IPROP_IN_LOOP(properties))
            node_p_free1(node->cases[i - 1].body.nodes, node->cases[i - 1].body.size);
    }

    if (node->ebody.size)
    {
        ires.value = ires_merge(&ires, interpret_body(&node->ebody, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                free(node->cases);
                free(node);
            }
            return ires;
        }
    }
    else
        ires.value.type = NULL_V;

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->cases);
        free(node);
    }

    return ires;
}

ires_t interpret_switch(switch_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            switch_n_free(node);

        return ires_fail(invalid_access_statement("switch statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    value_t value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->dbody.nodes, node->dbody.size);
            case_p_free(node->cases, node->size);
            free(node);
        }
        return ires;
    }

    unsigned long long i = 0;
    while (i < node->size)
    {
        value_t condition = ires_merge(&ires, interpret_node(&node->cases[i++].condition, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(&value);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->dbody.nodes, node->dbody.size);

                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->cases[node->size].body.nodes, node->cases[node->size].body.size);
                    node_free(&node->cases[node->size].condition);
                }
                free(node->cases);

                free(node);
            }

            return ires;
        }

        if (operate_equal_compare(&value, &condition))
        {
            value_free(&condition);
            value_free(&value);

            ires.value = ires_merge(&ires, interpret_body(&node->cases[i - 1].body, context, properties & IPROP_MASK));

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->dbody.nodes, node->dbody.size);

                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->cases[node->size].body.nodes, node->cases[node->size].body.size);
                    node_free(&node->cases[node->size].condition);
                }
                free(node->cases);

                free(node);
            }

            if (IRES_HAS_ERROR(ires.response))
                return ires;

            ires.value.poss = *poss;
            ires.value.pose = *pose;
            ires.value.context = context;

            return ires;
        }

        value_free(&condition);

        if (!IPROP_IN_LOOP(properties))
            node_p_free1(node->cases[i - 1].body.nodes, node->cases[i - 1].body.size);
    }

    if (node->dbody.size)
    {
        ires.value = ires_merge(&ires, interpret_body(&node->dbody, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                free(node->cases);
                free(node);
            }
            return ires;
        }
    }
    else
        ires.value.type = NULL_V;

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        free(node->cases);
        free(node);
    }

    return ires;
}

ires_t interpret_for(for_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            for_n_free(node);

        return ires_fail(invalid_access_statement("for statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    value_t start;
    if (node->start.type != NULL_N)
    {
        start = ires_merge(&ires, interpret_node(&node->start, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->end);
                free(node->iterator);
                free(node);
            }
            return ires;
        }

        if (start.type != INT_V && start.type != FLOAT_V && start.type != BOOL_V && start.type != CHAR_V)
        {
            value_free(&start);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->end);
                free(node->iterator);
                free(node);
            }

            return ires_fail(invalid_type("Start of for loop", "<int>, <float>, <bool> or <char>", start.type,
                &start.poss, &start.pose, start.context));
        }
    }
    else
    {
        start.type = INT_V;
        start.value.ptr = int_set_ull(0);
    }

    value_t end = ires_merge(&ires, interpret_node(&node->end, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(&start);

        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            free(node->iterator);
            free(node);
        }

        return ires;
    }

    if (end.type != INT_V && end.type != FLOAT_V && end.type != BOOL_V && end.type != CHAR_V)
    {
        value_free(&end);
        value_free(&start);

        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            free(node->iterator);
            free(node);
        }

        return ires_fail(invalid_type("End of for loop", "<int>, <float>, <bool> or <char>", end.type,
            &end.poss, &end.pose, end.context));
    }

    value_t step;
    if (node->step.type != NULL_N)
    {
        step = ires_merge(&ires, interpret_node(&node->step, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(&end);
            value_free(&start);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires;
        }

        if (step.type != INT_V && step.type != FLOAT_V && step.type != BOOL_V && step.type != CHAR_V)
        {
            value_free(&step);
            value_free(&end);
            value_free(&start);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(invalid_type("Step of for loop", "<int>, <float>, <bool> or <char>", step.type,
                &step.poss, &step.pose, step.context));
        }
    }
    else
    {
        step.type = BOOL_V;
        step.value.chr = 1;
    }

    char flag = 0;
    var_p var = table_ptr_var_set(&context->table, 0, node->iterator, 0, &start, &flag);
    if (flag == -1)
    {
        value_free(&step);
        value_free(&end);
        value_free(&start);

        // ill defined
        runtime_t error = const_variable(node->iterator, poss, pose, context);

        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }
    if (flag)
    {
        value_free(&step);
        value_free(&end);
        value_free(&start);

        runtime_t error = type_specified_variable(node->iterator, flag, poss, pose, context);

        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }

    value_t value;

    char sign = operate_sign(&step);
    if (sign)
        while (operate_greater_compare(&var->value, &end))
        {
            value = ires_merge(&ires, interpret_body(&node->body, context, IPROP_SET(0, 0, 1, 0)));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(&step);
                value_free(&end);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires;
            }

            if (IRES_LOOP_CONTINUE(ires.response))
                continue;

            if (IRES_LOOP_BREAK(ires.response))
                break;

            if (VAR_CONST(var->properties))
            {
                value_free(&step);
                value_free(&end);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires_fail(const_variable(var->name, &step.poss, &step.pose, step.context));
            }
            if (var->type != NULL_V && operate_success_type_change(&var->value, &step))
            {
                value_free(&step);
                value_free(&end);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires_fail(type_specified_variable(var->name, var->type, poss, pose, context));
            }

            operate_success(&var->value, &step);

            value_free(&ires.value);
            ires.value = value;
        }
    else
        while (operate_less_compare(&var->value, &end))
        {
            value = ires_merge(&ires, interpret_body(&node->body, context, IPROP_SET(0, 0, 1, 0)));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(&step);
                value_free(&end);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires;
            }

            if (IRES_LOOP_CONTINUE(ires.response))
                continue;

            if (IRES_LOOP_BREAK(ires.response))
                break;

            if (VAR_CONST(var->properties))
            {
                value_free(&step);
                value_free(&end);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires_fail(const_variable(var->name, &step.poss, &step.pose, step.context));
            }
            if (var->type != NULL_V && operate_success_type_change(&var->value, &step))
            {
                value_free(&step);
                value_free(&end);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires_fail(type_specified_variable(var->name, var->type, poss, pose, context));
            }

            operate_success(&var->value, &step);

            value_free(&ires.value);
            ires.value = value;
        }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        free(node->iterator);
        free(node);
    }

    ires.response = 0;
    return ires;
}

ires_t interpret_foreach(foreach_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            foreach_n_free(node);

        return ires_fail(invalid_access_statement("foreach statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    value_t iterable = ires_merge(&ires, interpret_node(&node->iterable, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }
        return ires;
    }

    if (iterable.type != STR_V && iterable.type != LIST_V && iterable.type != TUPLE_V)
    {
        value_free(&iterable);

        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(invalid_type("Iterable of foreach loop", "<str>, <list> or <tuple>", iterable.type,
            &iterable.poss, &iterable.pose, iterable.context));
    }

    unsigned long long size = operate_size(&iterable);

    char res = 0;
    value_t value;

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        value = operate_index(&iterable, i);
        res = table_var_set(&context->table, 0, node->iterator, 0, &value);
        if (res == -1)
        {
            value_free(&iterable);

            runtime_t error = const_variable(node->iterator, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }
        if (res)
        {
            value_free(&iterable);

            runtime_t error = type_specified_variable(node->iterator, res, poss, pose, context);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }

        value = ires_merge(&ires, interpret_body(&node->body, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(&iterable);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires;
        }

        if (IRES_LOOP_CONTINUE(ires.response))
            continue;

        if (IRES_LOOP_BREAK(ires.response))
            break;

        value_free(&ires.value);
        ires.value = value;
    }

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        free(node->iterator);
        free(node);
    }

    ires.response = 0;
    return ires;
}

ires_t interpret_loop(loop_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            loop_n_free(node);

        return ires_fail(invalid_access_statement("loop statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    value_t init = ires_merge(&ires, interpret_node(&node->init, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            node_free(&node->condition);
            free(node);
        }
        return ires;
    }

    value_free(&init);

    value_t condition, value, step;
    step.type = NULL_V;
    while (1)
    {
        condition = ires_merge(&ires, interpret_node(&node->condition, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (!value_is_true(&condition))
            break;

        value = ires_merge(&ires, interpret_body(&node->body, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (IRES_LOOP_BREAK(ires.response))
            break;

        if (!IRES_LOOP_CONTINUE(ires.response))
        {
            value_free(&ires.value);
            ires.value = value;
        }

        value_free(&step);

        step = ires_merge(&ires, interpret_node(&node->step, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }
    }

    value_free(&step);
    value_free(&condition);

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        node_free(&node->step);
        node_free(&node->condition);
        free(node);
    }

    ires.response = 0;
    return ires;
}

ires_t interpret_do_while(do_while_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            do_while_n_free(node);

        return ires_fail(invalid_access_statement("do-while statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    value_t condition, value;
    while (1)
    {
        value = ires_merge(&ires, interpret_body(&node->body, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (IRES_LOOP_BREAK(ires.response))
            break;

        if (!IRES_LOOP_CONTINUE(ires.response))
        {
            value_free(&ires.value);
            ires.value = value;
        }

        condition = ires_merge(&ires, interpret_node(&node->condition, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (!value_is_true(&condition))
            break;

        value_free(&condition);
    }

    value_free(&condition);

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        node_free(&node->condition);
        free(node);
    }

    ires.response = 0;
    return ires;
}

ires_t interpret_while(while_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            while_n_free(node);

        return ires_fail(invalid_access_statement("while statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value.type = NULL_V;

    value_t condition, value;
    while (1)
    {
        condition = ires_merge(&ires, interpret_node(&node->condition, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (!value_is_true(&condition))
            break;

        value = ires_merge(&ires, interpret_body(&node->body, context, IPROP_SET(0, 0, 1, 0)));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (IRES_LOOP_BREAK(ires.response))
            break;

        value_free(&condition);

        if (IRES_LOOP_BREAK(ires.response))
            continue;

        value_free(&ires.value);
        ires.value = value;
    }

    value_free(&condition);

    ires.value.poss = *poss;
    ires.value.pose = *pose;
    ires.value.context = context;

    if (!IPROP_IN_LOOP(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        node_free(&node->condition);
        free(node);
    }

    ires.response = 0;
    return ires;
}

ires_t interpret_try(try_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_IN_LOOP(properties))
            try_n_free(node);

        return ires_fail(invalid_access_statement("try statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    ires.value = ires_merge(&ires, interpret_body(&node->tbody, context, properties & IPROP_MASK));
    if (!IRES_HAS_ERROR(ires.response))
    {
        ires.value.poss = *poss;
        ires.value.pose = *pose;
        ires.value.context = context;

        if (!IPROP_IN_LOOP(properties))
        {
            node_p_free1(node->fbody.nodes, node->fbody.size);
            case_p_free(node->excepts, node->size);
            free(node);
        }

        return ires;
    }

    runtime_t error = ires.error;
    ires.response = 0;

    unsigned long long i;
    char* label = NULL;
    unsigned long long code;
    for (i = 0; i < node->size; i++)
    {
        ires.value = ires_merge(&ires, interpret_node(&node->excepts[i].condition, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            free(error.detail);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->fbody.nodes, node->fbody.size);

                while (node->size > i + 1)
                {
                    node->size--;
                    node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                    node_free(&node->excepts[node->size].condition);
                }
                node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                free(node->excepts);

                free(node);
            }

            return ires;
        }

        switch (ires.value.type)
        {
        case INT_V:
            if (int_sign(ires.value.value.ptr) < 0 || !int_fits_ull(ires.value.value.ptr))
            {
                int_free(ires.value.value.ptr);
                free(error.detail);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    while (node->size > i + 1)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                    free(node->excepts);

                    free(node);
                }

                return ires_fail(out_of_boundary("Error code", 0, ERROR_CODE_COUNT,
                    &ires.value.poss, &ires.value.pose, ires.value.context));
            }

            code = int_get_ull(ires.value.value.ptr);

            int_free(ires.value.value.ptr);
            break;
        case BOOL_V:
        case CHAR_V:
            code = ires.value.value.chr;
            break;
        case STR_V:
            label = ((str_p)ires.value.value.ptr)->str;
            break;
        default:
            value_free(&ires.value);
            free(error.detail);

            if (!IPROP_IN_LOOP(properties))
            {
                node_p_free1(node->fbody.nodes, node->fbody.size);

                while (node->size > i + 1)
                {
                    node->size--;
                    node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                    node_free(&node->excepts[node->size].condition);
                }
                node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                free(node->excepts);

                free(node);
            }

            return ires_fail(invalid_type("Exception", "<int>, <bool>, <char> or <str>", ires.value.type,
                &ires.value.poss, &ires.value.pose, ires.value.context));
        }

        if (!label)
        {
            if (code > ERROR_CODE_COUNT)
            {
                free(error.detail);

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    while (node->size > i + 1)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                    free(node->excepts);

                    free(node);
                }

                return ires_fail(out_of_boundary("Error code", 0, ERROR_CODE_COUNT,
                    &ires.value.poss, &ires.value.pose, ires.value.context));
            }

            if (code == ires.error.type)
            {
                free(error.detail);

                ires.value = ires_merge(&ires, interpret_body(&node->excepts[i].body, context, properties | IPROP_MASK));

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    while (node->size > i + 1)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    free(node->excepts);

                    free(node);
                }

                if (IRES_HAS_ERROR(ires.response))
                    return ires;

                ires.value.poss = *poss;
                ires.value.pose = *pose;
                ires.value.context = context;

                return ires;
            }
        }
        else
        {
            str_free(ires.value.value.ptr);

            if (!strcmp(label, error.detail) || !strcmp(label, runtime_labels[error.type]))
            {
                free(error.detail);

                ires.value = ires_merge(&ires, interpret_body(&node->excepts[i].body, context, properties | IPROP_MASK));

                if (!IPROP_IN_LOOP(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    while (node->size > i + 1)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    free(node->excepts);

                    free(node);
                }

                if (IRES_HAS_ERROR(ires.response))
                    return ires;

                ires.value.poss = *poss;
                ires.value.pose = *pose;
                ires.value.context = context;

                return ires;
            }
        }

        if (!IPROP_IN_LOOP(properties))
            node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
    }

    if (!IPROP_IN_LOOP(properties))
        free(node->excepts);

    if (node->fbody.size)
    {
        free(error.detail);

        ires.value = ires_merge(&ires, interpret_body(&node->fbody, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_IN_LOOP(properties))
                free(node);
            return ires;
        }

        ires.value.poss = *poss;
        ires.value.pose = *pose;
        ires.value.context = context;

        return ires;
    }

    return ires_fail(error);
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
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_statement("continue statement", poss, pose, context));

    if (!IPROP_IN_LOOP(properties))
        return ires_fail(outside_loop("Continue", poss, pose, context));

    ires_t ires;
    ires.response = IRES_RESPONSE_SET(0, 1, 0, 0);
    ires.value.type = NULL_V;

    return ires;
}

ires_t interpret_break(pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_statement("break statement", poss, pose, context));

    if (!IPROP_IN_LOOP(properties))
        return ires_fail(outside_loop("Break", poss, pose, context));

    ires_t ires;
    ires.response = IRES_RESPONSE_SET(0, 0, 1, 0);
    ires.value.type = NULL_V;

    return ires;
}
