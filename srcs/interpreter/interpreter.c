/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/operation.h>
#include <interpreter/dollar_func.h>
#include <interpreter/type_call.h>
#include <debugger/runtime_error.h>
#include <complex.h>
#include <str.h>
#include <array/list.h>
#include <structure/function.h>
#include <stdlib.h>
#include <string.h>
#include <lexer/token.h>

#define IPROP_SET(ptr, assign, not_free, in_func, in_loop) ((ptr) | (assign) << 1 | (not_free) << 2 | (in_func) << 3 | (in_loop) << 4)
#define IPROP_MASK 0b11100

#define VFA_IPROP 0b00001

#define IPROP_PTR(x) ((x) & 1)
#define IPROP_ASSIGN(x) ((x) >> 1 & 1)
#define IPROP_NOT_FREE(x) ((x) >> 2 & 1)
#define IPROP_IN_FUNC(x) ((x) >> 3 & 1)
#define IPROP_IN_LOOP(x) ((x) >> 4 & 1)

value_p ires_merge(ires_p ires, ires_t other);

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
    ires.value = NULL;

    node_p nodes_copy = nodes;

    while (nodes->type != NULL_N)
    {
        value_free(ires.value);

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

    ires.value = value;
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

value_p ires_merge(ires_p ires, ires_t other)
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
    case ACCESS_N:
        return interpret_access(node->value.ptr, &node->poss, &node->pose, context, properties);
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
    case FUNC_CALL_N:
        return interpret_func_call(node->value.ptr, &node->poss, &node->pose, context, properties);
    case STRUCT_DEF_N:
        return interpret_struct_def(node->value.ptr, &node->poss, &node->pose, context, properties);
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
    case RETURN_N:
        return interpret_return(node->value.ptr, &node->poss, &node->pose, context, properties);
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
    ires.value = NULL;

    unsigned long long i = 0;
    while (i < body->size)
    {
        value_free(ires.value);

        ires.value = ires_merge(&ires, interpret_node(body->nodes + i++, context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                while (body->size > i)
                    node_free(body->nodes + --body->size);

                free(body->nodes);
            }

            return ires;
        }

        if (IRES_FUNC_RETURN(ires.response) || IRES_LOOP_CONTINUE(ires.response) || IRES_LOOP_BREAK(ires.response))
            break;
    }

    if (!IPROP_NOT_FREE(properties))
        free(body->nodes);

    return ires;
}

ires_t interpret_none(pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(NONE_V, poss, pose, context));

    return ires_success(NULL);
}

ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            int_n_free(node);

        return ires_fail(invalid_access_object(INT_V, poss, pose, context));
    }

    int_p ptr = int_set_str(node->value, node->size, 10);

    if (!IPROP_NOT_FREE(properties))
        int_n_free(node);

    return ires_success(value_set1(INT_V, ptr));
}

ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            float_n_free(node);

        return ires_fail(invalid_access_object(FLOAT_V, poss, pose, context));
    }

    float_p ptr = float_set_str(node->value, node->size, 10);

    if (!IPROP_NOT_FREE(properties))
        float_n_free(node);

    return ires_success(value_set1(FLOAT_V, ptr));
}

ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            complex_n_free(node);

        return ires_fail(invalid_access_object(COMPLEX_V, poss, pose, context));
    }

    complex_p ptr = complex_set_str(NULL, 0, node->value, node->size, 10);

    if (!IPROP_NOT_FREE(properties))
        complex_n_free(node);

    return ires_success(value_set1(COMPLEX_V, ptr));
}

ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(BOOL_V, poss, pose, context));

    return ires_success(value_set2(BOOL_V, node));
}

ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_object(CHAR_V, poss, pose, context));

    return ires_success(value_set2(CHAR_V, node));
}

ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            str_n_free(node);

        return ires_fail(invalid_access_object(STR_V, poss, pose, context));
    }

    str_p ptr = str_set_str(node->value, node->size);

    if (!IPROP_NOT_FREE(properties))
        str_n_free(node);

    return ires_success(value_set1(STR_V, ptr));
}

ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (node && !IPROP_NOT_FREE(properties))
            list_n_free(node);

        return ires_fail(invalid_access_object(LIST_V, poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    if (!node)
    {
        ires.value = value_set1(LIST_V, list_set(NULL, 0));
        return ires;
    }

    value_p* elements = malloc(node->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_p element = ires_merge(&ires, interpret_node(&node->elements[i], context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                i++;
                while (node->size > i)
                    node_free(node->elements + --node->size);
                free(node->elements);
                i--;

                free(node);
            }

            while (i)
                value_free(elements[--i]);
            free(elements);

            return ires;
        }

        elements[i] = element;
    }

    ires.value = value_set1(LIST_V, list_set(elements, i));

    if (!IPROP_NOT_FREE(properties))
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
        if (!IPROP_NOT_FREE(properties))
            tuple_n_free(node);

        return ires_fail(invalid_access_object(TUPLE_V, poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p* elements = malloc(node->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_p element = ires_merge(&ires, interpret_node(&node->elements[i], context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                i++;
                while (node->size > i)
                    node_free(node->elements + --node->size);
                free(node->elements);
                i--;

                free(node);
            }

            while (i)
                value_free(elements[--i]);
            free(elements);

            return ires;
        }

        elements[i] = element;
    }

    ires.value = value_set1(TUPLE_V, tuple_set(elements, i));

    if (!IPROP_NOT_FREE(properties))
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

    return ires_success(value_set2(TYPE_V, node - OBJECT_TT + OBJECT_V));
}

ires_t interpret_binary_operation(binary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            binary_operation_n_free(node);

        return ires_fail(invalid_access_statement("binary operation", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p left = ires_merge(&ires, interpret_node(&node->left, context, properties));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->right);
            free(node);
        }

        return ires;
    }

    value_p right;

    char res;
    switch (node->operator)
    {
    case AND_T:
    case AND_TK:
        res = value_is_true(left);
        if (res)
        {
            right = ires_merge(&ires, interpret_node(&node->right, context, properties));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(left);

                if (!IPROP_NOT_FREE(properties))
                    free(node);

                return ires;
            }

            res = value_is_true(right);

            value_free(right);
        }
        else if (!IPROP_NOT_FREE(properties))
            node_free(&node->right);

        value_free(left);

        ires.value = value_set2(BOOL_V, res);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    case OR_T:
    case OR_TK:
        res = value_is_true(left);
        if (!res)
        {
            right = ires_merge(&ires, interpret_node(&node->right, context, properties));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(left);

                if (!IPROP_NOT_FREE(properties))
                    free(node);

                return ires;
            }

            res = value_is_true(right);

            value_free(right);
        }
        else if (!IPROP_NOT_FREE(properties))
            node_free(&node->right);

        value_free(left);

        ires.value = value_set2(BOOL_V, res);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    right = ires_merge(&ires, interpret_node(&node->right, context, properties));
    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(left);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    switch (node->operator)
    {
    case PLUS_T:
        ires = operate_add(left, right, poss, pose, context);
        break;
    case MINUS_T:
        ires = operate_subtract(left, right, poss, pose, context, &node->right.poss, &node->right.pose);
        break;
    case MULTIPLY_T:
        ires = operate_multiply(left, right, poss, pose, context);
        break;
    case DIVIDE_T:
        ires = operate_divide(left, right, poss, pose, context, &node->right.poss, &node->right.pose);
        break;
    case MODULO_T:
        ires = operate_modulo(left, right, poss, pose, context, &node->right.poss, &node->right.pose);
        break;
    case QUOTIENT_T:
        ires = operate_quotient(left, right, poss, pose, context, &node->right.poss, &node->right.pose);
        break;
    case POWER_T:
        ires = operate_power(left, right, poss, pose, context);
        break;
    case B_AND_T:
        ires = operate_b_and(left, right, poss, pose, context);
        break;
    case B_OR_T:
        ires = operate_b_or(left, right, poss, pose, context);
        break;
    case B_XOR_T:
        ires = operate_b_xor(left, right, poss, pose, context);
        break;
    case LSHIFT_T:
        ires = operate_lshift(left, right, poss, pose, context);
        break;
    case RSHIFT_T:
        ires = operate_rshift(left, right, poss, pose, context);
        break;
    case EQUAL_T:
        ires = operate_equal(left, right);
        break;
    case NEQUAL_T:
        ires = operate_nequal(left, right);
        break;
    case EX_EQUAL_T:
        ires = operate_ex_equal(left, right);
        break;
    case EX_NEQUAL_T:
        ires = operate_ex_nequal(left, right);
        break;
    case LESS_T:
        ires = operate_less(left, right, poss, pose, context);
        break;
    case GREATER_T:
        ires = operate_greater(left, right, poss, pose, context);
        break;
    case LESS_EQ_T:
        ires = operate_less_eq(left, right, poss, pose, context);
        break;
    case GREATER_EQ_T:
        ires = operate_greater_eq(left, right, poss, pose, context);
        break;
    case XOR_T:
    case XOR_TK:
        ires = operate_xor(left, right);
        break;
    case IN_TK:
        ires = operate_contain(left, right, poss, pose, context);
        break;
    case IS_TK:
        ires = operate_is(left, right);
        break;
    case ARE_TK:
        ires = operate_are(left, right, poss, pose, context, &node->left.poss, &node->right.pose);
        break;
    }

    if (!IPROP_NOT_FREE(properties))
        free(node);

    return ires;
}

ires_t interpret_unary_operation(unary_operation_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            unary_operation_n_free(node);

        return ires_fail(invalid_access_statement("unary operation", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p operand = ires_merge(&ires, interpret_node(&node->operand, context, properties));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    switch (node->operator)
    {
    case PLUS_T:
        ires = operate_positive(operand);
        break;
    case MINUS_T:
        ires = operate_negate(operand, poss, pose, context);
        break;
    case B_NOT_T:
        ires = operate_b_not(operand, poss, pose, context);
        break;
    case NOT_T:
    case NOT_TK:
        ires = operate_not(operand);
        break;
    }

    if (!IPROP_NOT_FREE(properties))
        free(node);

    return ires;
}

ires_t interpret_ternary_condition(ternary_condition_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_p condition = ires_merge(&ires, interpret_node(&node->condition, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->left);
            node_free(&node->right);
            free(node);
        }

        return ires;
    }

    if (value_is_true(condition))
    {
        if (!IPROP_NOT_FREE(properties))
            node_free(&node->right);

        if (node->left.type != NULL_N)
        {
            ires.value = ires_merge(&ires, interpret_node(&node->left, context, properties));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(condition);

                if (!IPROP_NOT_FREE(properties))
                    free(node);

                return ires;
            }
        }
        else
            ires.value = NULL;
    }
    else
    {
        if (!IPROP_NOT_FREE(properties))
            node_free(&node->left);

        ires.value = ires_merge(&ires, interpret_node(&node->right, context, properties));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(condition);

            if (!IPROP_NOT_FREE(properties))
                free(node);

            return ires;
        }
    }

    value_free(condition);

    if (!IPROP_NOT_FREE(properties))
        free(node);

    return ires;
}

ires_t interpret_subscript(subscript_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_p value = ires_merge(&ires, interpret_node(&node->value, context, properties));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->pos);
            free(node);
        }

        return ires;
    }

    value_p pos = ires_merge(&ires, interpret_node(&node->pos, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(value);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    if (IPROP_PTR(properties))
    {
        if (IRES_CHAR_PTR(ires.response))
        {
            value_free(pos);

            runtime_t error = invalid_type("Operand", "<str> or <list>", CHAR_V,
                &node->value.poss, &node->value.pose, context);

            if (!IPROP_NOT_FREE(properties))
                free(node);

            return ires_fail(error);
        }

        value_p* ptr;
        if (IRES_VAL_PTR(ires.response))
            ptr = (value_p*)value;
        else
            ptr = &((var_p)value)->value;

        ires = operate_subscript_ptr(ptr, pos, context, &node->value.poss, &node->value.pose, &node->pos.poss, &node->pos.pose);
    }
    else
        ires = operate_subscript(value, pos, context, &node->value.poss, &node->value.pose, &node->pos.poss, &node->pos.pose);

    if (!IPROP_NOT_FREE(properties))
        free(node);

    return ires;
}

ires_t interpret_access(access_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_p value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->property);
            free(node);
        }

        return ires;
    }

    if (!value)
    {
        runtime_t error = invalid_type("Value", "<func> or <struct>", NONE_V, &node->value.poss, &node->value.pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->property);
            free(node);
        }

        return ires_fail(error);
    }

    context_p access;
    switch (value->type)
    {
    case FUNC_V:
        access = &func_context(value->value.ptr);
        break;
    case STRUCT_V:
        access = value->value.ptr;
        break;
    default:
        value_free(value);

        runtime_t error = invalid_type("Value", "<func> or <struct>", value->type, &node->value.poss, &node->value.pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->property);
            free(node);
        }

        return ires_fail(error);
    }

    ires.value = ires_merge(&ires, interpret_node(&node->property, access, properties));
    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(value);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    value_free(value);

    if (!IPROP_NOT_FREE(properties))
        free(node);

    return ires;
}

ires_t interpret_var_assign(var_assign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    if (node->type)
        node->type -= OBJECT_TT - OBJECT_V;

    if (node->value.type)
    {
        ires.value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                free(node->name);
                free(node);
            }

            return ires;
        }

        if (node->type != NULL_N && node->type != ires.value->type)
        {
            runtime_t error = type_match(node->type, ires.value->type, poss, pose, context);

            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
    }
    else
        ires.value = NULL;

    context_p copy = context;

    if (PROP_GLOBAL(node->properties))
        while (copy->parent)
            copy = copy->parent;

    char flag = 0;
    if (IPROP_PTR(properties))
    {
        var_p ptr = table_ptr_set(&copy->table, node->properties, node->name, node->type, ires.value, &flag);
        if (flag == -1)
        {
            value_free(ires.value);

            runtime_t error = const_variable(node->name, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
        if (flag)
        {
            value_free(ires.value);

            runtime_t error = type_specified_variable(node->name, flag, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }

        ires.value = (value_p)ptr;
    }
    else
    {
        flag = table_var_set(&copy->table, node->properties, node->name, node->type, ires.value);
        if (flag == -1)
        {
            value_free(ires.value);

            runtime_t error = const_variable(node->name, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
        if (flag)
        {
            value_free(ires.value);

            runtime_t error = type_specified_variable(node->name, flag, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }

        value_copy(ires.value);
    }

    if (!IPROP_NOT_FREE(properties))
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

    value_p ptr = ires_merge(&ires, interpret_node(&node->var, context, properties | VFA_IPROP));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    value_p res = NULL;
    if (node->properties)
    {
        if (IPROP_PTR(properties))
            res = ptr;
        else
        {
            if (IRES_CHAR_PTR(ires.response))
                res = value_set2(CHAR_V, *(char*)ptr);
            else if (IRES_VAL_PTR(ires.response))
            {
                res = *(value_p*)ptr;
                value_copy(res);
            }
            else
            {
                res = ((var_p)ptr)->value;
                value_copy(res);
            }
        }
    }

    char response = ires.response;

    switch (node->operator)
    {
    case INCREMENT_T:
        if (IRES_CHAR_PTR(ires.response))
            (*(char*)ptr)++;
        else if (IRES_VAL_PTR(ires.response))
            ires = operate_increment(*(value_p*)ptr, poss, pose, context);
        else
            ires = operate_increment(((var_p)ptr)->value, poss, pose, context);
        break;
    case DECREMENT_T:
        if (IRES_CHAR_PTR(ires.response))
            (*(char*)ptr)--;
        else if (IRES_VAL_PTR(ires.response))
            ires = operate_decrement(*(value_p*)ptr, poss, pose, context);
        else
            ires = operate_decrement(((var_p)ptr)->value, poss, pose, context);
        break;
    }

    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_PTR(properties))
            value_free(res);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    if (IRES_VAL_PTR(response))
        *(value_p*)ptr = ires.value;
    else if (!IRES_CHAR_PTR(response))
        ((var_p)ptr)->value = ires.value;

    if (node->properties)
        ires.value = res;
    else
    {
        if (IPROP_PTR(properties))
            ires.value = ptr;
        else
        {
            if (IRES_CHAR_PTR(response))
                ires.value = value_set2(CHAR_V, *(char*)ptr);
            else
                value_copy(ires.value);
        }
    }

    if (!IPROP_NOT_FREE(properties))
        free(node);

    ires.response = response & IRES_RESPONSE_SET(0, 1, 1, 1, IPROP_PTR(properties), IPROP_PTR(properties));
    return ires;
}

ires_t interpret_var_reassign(var_reassign_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    value_p ptr = ires_merge(&ires, interpret_node(&node->var, context,
        properties | IPROP_SET(1, node->operator == ASSIGN_T, 0, 0, 0)));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_free(&node->value);
            free(node);
        }
        return ires;
    }

    char ptype;
    if (IRES_CHAR_PTR(ires.response))
        ptype = CHAR_V;
    else if (IRES_VAL_PTR(ires.response))
        ptype = NONE_V;
    else
        ptype = ((var_p)ptr)->type;

    value_p left = NULL;
    if (node->operator != ASSIGN_T)
    {
        if (IRES_CHAR_PTR(ires.response))
            left = value_set2(CHAR_V, *(char*)ptr);
        else if (IRES_VAL_PTR(ires.response))
        {
            left = *(value_p*)ptr;
            value_copy(left);
        }
        else
        {
            left = ((var_p)ptr)->value;
            value_copy(left);
        }
    }

    ires.value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
            free(node);
        return ires;
    }

    char response = ires.response;

    switch (node->operator)
    {
    case ASSIGN_T:
        goto ret;
    case PLUS_EQ_T:
        ires = operate_add(left, ires.value, poss, pose, context);
        break;
    case MINUS_EQ_T:
        ires = operate_subtract(left, ires.value, poss, pose, context, &node->value.poss, &node->value.pose);
        break;
    case MULTIPLY_EQ_T:
        ires = operate_multiply(left, ires.value, poss, pose, context);
        break;
    case DIVIDE_EQ_T:
        ires = operate_divide(left, ires.value, poss, pose, context, &node->value.poss, &node->value.pose);
        break;
    case MODULO_EQ_T:
        ires = operate_modulo(left, ires.value, poss, pose, context, &node->value.poss, &node->value.pose);
        break;
    case QUOTIENT_EQ_T:
        ires = operate_quotient(left, ires.value, poss, pose, context, &node->value.poss, &node->value.pose);
        break;
    case POWER_EQ_T:
        ires = operate_power(left, ires.value, poss, pose, context);
        break;
    case B_AND_EQ_T:
        ires = operate_b_and(left, ires.value, poss, pose, context);
        break;
    case B_OR_EQ_T:
        ires = operate_b_or(left, ires.value, poss, pose, context);
        break;
    case B_XOR_EQ_T:
        ires = operate_b_xor(left, ires.value, poss, pose, context);
        break;
    case LSHIFT_EQ_T:
        ires = operate_lshift(left, ires.value, poss, pose, context);
        break;
    case RSHIFT_EQ_T:
        ires = operate_rshift(left, ires.value, poss, pose, context);
        break;
    }

    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

ret:
    if (ptype && ires.value && ptype != ires.value->type)
    {
        char vtype = ires.value->type;

        value_free(left);
        value_free(ires.value);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires_fail(assign_type_specified_variable(vtype, ptype, poss, pose, context));
    }

    if (IRES_CHAR_PTR(response))
    {
        *(char*)ptr = ires.value->value.chr;

        if (!IPROP_NOT_FREE(properties))
            free(node);

        if (IPROP_PTR(properties))
        {
            value_free_shell(ires.value);
            ires.value = (value_p)ptr;
        }

        ires.response = response & IRES_RESPONSE_SET(0, 1, 1, 1, IPROP_PTR(properties), IPROP_PTR(properties));
        return ires;
    }
    if (IRES_VAL_PTR(response))
    {
        value_free(*(value_p*)ptr);
        *(value_p*)ptr = ires.value;

        if (!IPROP_NOT_FREE(properties))
            free(node);

        if (IPROP_PTR(properties))
            ires.value = (value_p)ptr;
        else
            value_copy(ires.value);

        ires.response = response & IRES_RESPONSE_SET(0, 1, 1, 1, IPROP_PTR(properties), IPROP_PTR(properties));
        return ires;
    }

    value_free(((var_p)ptr)->value);
    ((var_p)ptr)->value = ires.value;

    if (!IPROP_NOT_FREE(properties))
        free(node);

    if (IPROP_PTR(properties))
        ires.value = (value_p)ptr;
    else
        value_copy(ires.value);

    ires.response = response & IRES_RESPONSE_SET(0, 1, 1, 1, IPROP_PTR(properties), IPROP_PTR(properties));
    return ires;
}

ires_t interpret_var_access(char* node, pos_p poss, pos_p pose, context_p context, char properties)
{
    ires_t ires;
    ires.response = 0;

    if (IPROP_PTR(properties))
    {
        char flag = 0;
        if (IPROP_ASSIGN(properties))
            ires.value = (value_p)table_ptr_get(&context->table, node, &flag);
        else
            ires.value = (value_p)context_ptr_get(context, node, &flag);

        if (flag)
        {
            runtime_t error = const_variable(node, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
                free(node);

            return ires_fail(error);
        }

        if (!ires.value)
        {
            if (IPROP_ASSIGN(properties))
            {
                ires.value = (value_p)table_ptr_add(&context->table, node);

                if (!IPROP_NOT_FREE(properties))
                    free(node);

                return ires;
            }

            runtime_t error = not_defined(node, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
                free(node);

            return ires_fail(error);
        }
    }
    else
    {
        ires.value = context_var_get(context, node);
        if (!ires.value)
        {
            runtime_t error = not_defined(node, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
                free(node);

            return ires_fail(error);
        }

        ires.value->ref++;
    }

    if (!IPROP_NOT_FREE(properties))
        free(node);

    return ires;
}

ires_t interpret_func_def(func_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            func_def_n_free(node);

        return ires_fail(invalid_access_statement("function definition", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    if (node->type)
        node->type -= OBJECT_TT - OBJECT_V;

    table_t table = table_set(FUNC_CONTEXT_SIZE + node->size);

    arg_value_p args = malloc(node->size * sizeof(arg_value_t));
    unsigned long long size = 0;

    unsigned long long min_size = 0;
    for (; table.size < node->size; table.size++)
    {
        table.vars[table.size].properties = 8;

        if (node->args[table.size].type)
            node->args[table.size].type -= OBJECT_TT - OBJECT_V;

        table.vars[table.size].type = node->args[table.size].type;

        if (node->args[table.size].value.type != NULL_N)
        {
            args[size].value = ires_merge(&ires, interpret_node(&node->args[table.size].value, context, properties & IPROP_MASK));
            if (IRES_HAS_ERROR(ires.response))
            {
                arg_value_free(args, size);
                if (!size)
                    free(args);

                table_delete(&table);

                if (!IPROP_NOT_FREE(properties))
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

            args[size].index = table.size;

            if (table.vars[table.size].type && args[size].value && table.vars[table.size].type != args[size].value->type)
            {
                runtime_t error = type_match(table.vars[table.size].type, args[size++].value->type,
                    &node->args[table.size].value.poss, &node->args[table.size].value.pose, context);

                arg_value_free(args, size);

                table_delete(&table);

                if (!IPROP_NOT_FREE(properties))
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

                return ires_fail(error);
            }

            size++;
        }
        else
            min_size = table.size + 1;

        table.vars[table.size].value = NULL;

        table.vars[table.size].name = malloc(strlen(node->args[table.size].name) + 1);
        strcpy(table.vars[table.size].name, node->args[table.size].name);

        if (!IPROP_NOT_FREE(properties))
            free(node->args[table.size].name);
    }

    if (!size)
        free(args);
    else if (size != node->size)
        args = realloc(args, size * sizeof(arg_value_t));

    context_p copy = context;

    if (PROP_GLOBAL(node->properties))
        while (copy->parent)
            copy = copy->parent;

    body_t body;
    body.size = node->body.size;

    if (IPROP_NOT_FREE(properties))
        body.nodes = node_p_copy(node->body.nodes, node->body.size);
    else
        body.nodes = node->body.nodes;

    if (node->name)
    {
        char* cname = malloc(strlen(node->name) + 1);
        strcpy(cname, node->name);

        char* name = malloc(strlen(node->name) + 1);
        strcpy(name, node->name);

        context_t func_context = context_set1(cname, copy, poss, &table, copy->fname);

        func_p func = func_set(node->type, min_size, node->size, args, size, &func_context, &body);
        ires.value = value_set1(FUNC_V, func);

        char res = table_var_set(&copy->table, node->properties, name, 0, ires.value);
        if (res == -1)
        {
            func_free(func);
            free(name);

            runtime_t error = const_variable(node->name, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->args);
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
        if (res)
        {
            func_free(func);
            free(name);

            runtime_t error = type_specified_variable(node->name, res, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->args);
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }

        value_copy(ires.value);
    }
    else
    {
        context_t func_context = context_set1(NULL, copy, poss, &table, copy->fname);

        func_p func = func_set(node->type, min_size, node->size, args, size, &func_context, &body);
        ires.value = value_set1(FUNC_V, func);
    }

    if (!IPROP_NOT_FREE(properties))
    {
        free(node->args);
        free(node->name);
        free(node);
    }

    return ires;
}

ires_t interpret_func_call(func_call_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            func_call_n_free(node);

        return ires_fail(invalid_access_statement("function call", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p func = ires_merge(&ires, interpret_node(&node->func, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            arg_access_p_free(node->args, node->size);
            free(node);
        }

        return ires;
    }

    if (!func)
    {
        runtime_t error = invalid_type("Value", "<type> or <func>", NONE_V, &node->func.poss, &node->func.pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            arg_access_p_free(node->args, node->size);
            free(node);
        }

        return ires_fail(error);
    }
    if (func->type != TYPE_V && func->type != FUNC_V)
    {
        runtime_t error = invalid_type("Value", "<type> or <func>", func->type, &node->func.poss, &node->func.pose, context);

        value_free(func);

        if (!IPROP_NOT_FREE(properties))
        {
            arg_access_p_free(node->args, node->size);
            free(node);
        }

        return ires_fail(error);
    }

    if (func->type == TYPE_V)
    {
        // ill defined
        switch (func->value.chr)
        {
        case OBJECT_V:
            ires = object_call(node);
            break;
        }

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    func_p func_v = func->value.ptr;

    if (node->size < func_v->min_size || node->size > func_v->max_size)
    {
        runtime_t error = invalid_arg_number_function(func_v->context.name, func_v->min_size, func_v->max_size, node->size,
            poss, pose, context);

        value_free_type(func, func);

        if (!IPROP_NOT_FREE(properties))
        {
            arg_access_p_free(node->args, node->size);
            free(node);
        }

        return ires_fail(error);
    }

    if (!func_v->running)
        func_table_free_args(&func_v->context.table, func_v->max_size);

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        ires.value = ires_merge(&ires, interpret_node(&node->args[i].value, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free_type(func, func);

            if (!IPROP_NOT_FREE(properties))
            {
                free(node->args[i].name);

                i++;
                while (node->size > i)
                {
                    node_free(&node->args[node->size].value);
                    free(node->args[--node->size].name);
                }
                free(node->args);

                free(node);
            }

            return ires;
        }

        if (!node->args[i].name)
        {
            if (func_v->context.table.vars[i].type && ires.value->type &&
                func_v->context.table.vars[i].type != ires.value->type)
            {
                runtime_t error = type_match(func_v->context.table.vars[i].type, ires.value->type,
                    &node->args[i].value.poss, &node->args[i].value.pose, context);

                value_free(ires.value);

                value_free_type(func, func);

                if (!IPROP_NOT_FREE(properties))
                {
                    i++;
                    while (node->size > i)
                    {
                        node_free(&node->args[node->size].value);
                        free(node->args[--node->size].name);
                    }
                    free(node->args);

                    free(node);
                }

                return ires_fail(error);
            }

            func_v->context.table.vars[i].value = ires.value;
        }
        else
        {
            char flag = 0;
            var_p ptr = table_ptr_get(&func_v->context.table, node->args[i].name, &flag);

            if (flag)
            {
                runtime_t error = const_variable(node->args[i].name, poss, pose, context);

                value_free(ires.value);

                value_free_type(func, func);

                if (!IPROP_NOT_FREE(properties))
                {
                    free(node->args[i].name);

                    i++;
                    while (node->size > i)
                    {
                        node_free(&node->args[node->size].value);
                        free(node->args[--node->size].name);
                    }
                    free(node->args);

                    free(node);
                }

                return ires_fail(error);
            }
            if (!ptr)
            {
                runtime_t error = not_defined(node->args[i].name, poss, pose, context);

                value_free(ires.value);

                value_free_type(func, func);

                if (!IPROP_NOT_FREE(properties))
                {
                    free(node->args[i].name);

                    i++;
                    while (node->size > i)
                    {
                        node_free(&node->args[node->size].value);
                        free(node->args[--node->size].name);
                    }
                    free(node->args);

                    free(node);
                }

                return ires_fail(error);
            }

            if (ptr->type != NONE_V && ires.value && ptr->type != ires.value->type)
            {
                runtime_t error = type_match(ptr->type, ires.value->type,
                    &node->args[i].value.poss, &node->args[i].value.pose, context);

                value_free(ires.value);

                value_free_type(func, func);

                if (!IPROP_NOT_FREE(properties))
                {
                    i++;
                    while (node->size > i)
                    {
                        node_free(&node->args[node->size].value);
                        free(node->args[--node->size].name);
                    }
                    free(node->args);

                    free(node);
                }

                return ires_fail(error);
            }

            value_free(ptr->value);
            ptr->value = ires.value;
        }
    }

    for (i = 0; i < func_v->size; i++)
        if (!func_v->context.table.vars[func_v->args[i].index].value)
        {
            func_v->context.table.vars[func_v->args[i].index].value = func_v->args[i].value;
            value_copy(func_v->args[i].value);
        }

    func_v->running = 1;
    ires.value = ires_merge(&ires, interpret_body(&func_v->body, &func_v->context, IPROP_SET(0, 0, 1, 1, 0)));
    func_v->running = 0;

    if (IRES_HAS_ERROR(ires.response))
    {
        if (func->ref)
        {
            func->ref--;
            func_table_free(&func_v->context.table, func_v->max_size);
        }
        else
            value_free_type(func, func);

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    if (!IPROP_NOT_FREE(properties))
        free(node);

    if (func_v->type && ires.value && func_v->type != ires.value->type)
    {
        runtime_t error = invalid_type("Return value", value_labels[func_v->type], ires.value->type, poss, pose, context);

        value_free_type(func, func);

        return ires_fail(error);
    }

    value_copy(ires.value);

    if (func->ref)
    {
        func->ref--;
        func_table_free(&func_v->context.table, func_v->max_size);
    }
    else
        value_free_type(func, func);

    ires.response = 0;
    return ires;
}

ires_t interpret_class_def(class_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{

}

ires_t interpret_struct_def(struct_def_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            struct_def_n_free(node);

        return ires_fail(invalid_access_statement("struct definition", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    context_p copy = context;

    if (PROP_GLOBAL(node->properties))
        while (copy->parent)
            copy = copy->parent;

    char* cname;
    if (node->name)
    {
        cname = malloc(strlen(node->name) + 1);
        strcpy(cname, node->name);
    }
    else
        cname = NULL;

    table_t table = table_set(STRUCT_CONTEXT_SIZE);

    context_p struct_context = malloc(sizeof(context_t));
    *struct_context = context_set1(cname, copy, poss, &table, copy->fname);

    value_p value = ires_merge(&ires, interpret_body(&node->body, struct_context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        context_free(struct_context);

        if (!IPROP_NOT_FREE(properties))
        {
            free(node->name);
            free(node);
        }

        return ires;
    }

    value_free(value);

    if (node->name)
    {
        char* name = malloc(strlen(node->name) + 1);
        strcpy(name, node->name);

        ires.value = value_set1(STRUCT_V, struct_context);

        char res = table_var_set(&copy->table, node->properties, name, 0, ires.value);
        if (res == -1)
        {
            context_free(struct_context);
            free(name);

            runtime_t error = const_variable(node->name, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }
        if (res)
        {
            context_free(struct_context);
            free(name);

            runtime_t error = type_specified_variable(node->name, res, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->name);
                free(node);
            }

            return ires_fail(error);
        }

        value_copy(ires.value);
    }
    else
        ires.value = value_set1(STRUCT_V, struct_context);

    if (!IPROP_NOT_FREE(properties))
    {
        free(node->name);
        free(node);
    }

    return ires;
}

ires_t interpret_dollar_func_call(dollar_func_call_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            dollar_func_call_n_free(node);

        return ires_fail(invalid_access_statement("dollar-function call", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p* args = malloc(node->size * sizeof(value_p));

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        value_p arg = ires_merge(&ires, interpret_node(&node->args[i], context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                i++;
                while (node->size > i)
                    node_free(node->args + --node->size);
                free(node->args);
                i--;

                free(node->name);
                free(node);
            }

            while (i)
                value_free(args[--i]);
            free(args);

            return ires;
        }

        args[i] = arg;
    }

    unsigned char id = 0;
    ires.value = ires_merge(&ires, handle_dollar_func(&id, node->name, args, i, poss, pose, context));

    if (id)
    {
        ires.error.poss = node->args[--id].poss;
        ires.error.pose = node->args[id].pose;
    }

    while (i)
        value_free(args[--i]);
    free(args);

    if (!IPROP_NOT_FREE(properties))
    {
        free(node->args);
        free(node->name);
        free(node);
    }

    return ires;
}

ires_t interpret_if(if_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            if_n_free(node);

        return ires_fail(invalid_access_statement("if statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    unsigned long long i = 0;
    while (i < node->size)
    {
        value_p condition = ires_merge(&ires, interpret_node(&node->cases[i++].condition, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
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

        if (value_is_true(condition))
        {
            value_free(condition);

            ires.value = ires_merge(&ires, interpret_body(&node->cases[i - 1].body, context, properties & IPROP_MASK));

            if (!IPROP_NOT_FREE(properties))
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

        value_free(condition);

        if (!IPROP_NOT_FREE(properties))
            node_p_free1(node->cases[i - 1].body.nodes, node->cases[i - 1].body.size);
    }

    if (node->ebody.size)
    {
        ires.value = ires_merge(&ires, interpret_body(&node->ebody, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                free(node->cases);
                free(node);
            }
            return ires;
        }
    }
    else
        ires.value = NULL;

    if (!IPROP_NOT_FREE(properties))
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
        if (!IPROP_NOT_FREE(properties))
            switch_n_free(node);

        return ires_fail(invalid_access_statement("switch statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    value_p value = ires_merge(&ires, interpret_node(&node->value, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
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
        value_p condition = ires_merge(&ires, interpret_node(&node->cases[i++].condition, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(value);

            if (!IPROP_NOT_FREE(properties))
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

        if (operate_equal_compare(value, condition))
        {
            value_free(condition);
            value_free(value);

            ires.value = ires_merge(&ires, interpret_body(&node->cases[i - 1].body, context, properties & IPROP_MASK));

            if (!IPROP_NOT_FREE(properties))
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

        value_free(condition);

        if (!IPROP_NOT_FREE(properties))
            node_p_free1(node->cases[i - 1].body.nodes, node->cases[i - 1].body.size);
    }

    if (node->dbody.size)
    {
        ires.value = ires_merge(&ires, interpret_body(&node->dbody, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                free(node->cases);
                free(node);
            }

            return ires;
        }
    }
    else
        ires.value = NULL;

    if (!IPROP_NOT_FREE(properties))
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
        if (!IPROP_NOT_FREE(properties))
            for_n_free(node);

        return ires_fail(invalid_access_statement("for statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value = NULL;

    value_p start;
    if (node->start.type != NULL_N)
    {
        start = ires_merge(&ires, interpret_node(&node->start, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->end);
                free(node->iterator);
                free(node);
            }
            return ires;
        }

        if (!start)
        {
            runtime_t error = invalid_type("Start of for loop", "<int>, <float>, <bool> or <char>", NONE_V,
                &node->start.poss, &node->start.pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->end);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }
        if (start->type != INT_V && start->type != FLOAT_V && start->type != BOOL_V && start->type != CHAR_V)
        {
            runtime_t error = invalid_type("Start of for loop", "<int>, <float>, <bool> or <char>", start->type,
                &node->start.poss, &node->start.pose, context);

            value_free(start);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->end);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }
    }
    else
        start = value_set1(INT_V, int_set_ull(0));

    value_p end = ires_merge(&ires, interpret_node(&node->end, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        value_free(start);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            free(node->iterator);
            free(node);
        }

        return ires;
    }

    if (!start)
    {
        runtime_t error = invalid_type("End of for loop", "<int>, <float>, <bool> or <char>", NONE_V,
            &node->end.poss, &node->end.pose, context);

        value_free(start);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }
    if (end->type != INT_V && end->type != FLOAT_V && end->type != BOOL_V && end->type != CHAR_V)
    {
        runtime_t error = invalid_type("End of for loop", "<int>, <float>, <bool> or <char>", end->type,
            &node->end.poss, &node->end.pose, context);

        value_free(end);
        value_free(start);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }

    value_p step;
    if (node->step.type != NULL_N)
    {
        step = ires_merge(&ires, interpret_node(&node->step, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(end);
            value_free(start);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires;
        }

        if (!step)
        {
            runtime_t error = invalid_type("Step of for loop", "<int>, <float>, <bool> or <char>", NONE_V,
                &node->step.poss, &node->step.pose, context);

            value_free(end);
            value_free(start);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }
        if (step->type != INT_V && step->type != FLOAT_V && step->type != BOOL_V && step->type != CHAR_V)
        {
            runtime_t error = invalid_type("Step of for loop", "<int>, <float>, <bool> or <char>", step->type,
                &node->step.poss, &node->step.pose, context);

            value_free(step);
            value_free(end);
            value_free(start);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }
    }
    else
        step = value_set2(BOOL_V, 1);

    char flag = 0;
    var_p var = table_ptr_set(&context->table, 0, node->iterator, 0, start, &flag);

    if (flag == -1)
    {
        value_free(step);
        value_free(end);
        value_free(start);

        // ill defined
        runtime_t error = const_variable(node->iterator, poss, pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }
    if (flag)
    {
        value_free(step);
        value_free(end);
        value_free(start);

        runtime_t error = type_specified_variable(node->iterator, flag, poss, pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }

    if (var->type != NONE_V && operate_success_type_change(var->value, step))
    {
        value_free(step);
        value_free(end);

        runtime_t error = type_specified_variable(var->name, var->type, &node->step.poss, &node->start.pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }

    value_p value;
    value_p tmp;

    char sign = operate_sign(step);
    if (!sign)
        while (operate_less_compare(var->value, end))
        {
            value = ires_merge(&ires, interpret_body(&node->body, context,
                IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(ires.value);
                value_free(step);
                value_free(end);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires;
            }

            if (IRES_FUNC_RETURN(ires.response))
            {
                value_free(ires.value);
                ires.value = value;

                break;
            }

            if (IRES_LOOP_CONTINUE(ires.response))
                goto success;

            if (IRES_LOOP_BREAK(ires.response))
                break;

            if (VAR_CONST(var->properties))
            {
                value_free(value);
                value_free(ires.value);
                value_free(step);
                value_free(end);

                runtime_t error = const_variable(var->name, &node->step.poss, &node->step.pose, context);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires_fail(error);
            }

success:
            tmp = ires_merge(&ires, operate_success(var->value, step, poss, pose, context));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(value);
                value_free(ires.value);
                value_free(step);
                value_free(end);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires;
            }

            var->value = tmp;

            value_free(ires.value);
            ires.value = value;
        }
    else
        while (operate_greater_compare(var->value, end))
        {
            value = ires_merge(&ires, interpret_body(&node->body, context,
                IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(ires.value);
                value_free(step);
                value_free(end);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires;
            }

            if (IRES_FUNC_RETURN(ires.response))
            {
                value_free(ires.value);
                ires.value = value;

                break;
            }

            if (IRES_LOOP_CONTINUE(ires.response))
                goto regress;

            if (IRES_LOOP_BREAK(ires.response))
                break;

            if (VAR_CONST(var->properties))
            {
                value_free(value);
                value_free(ires.value);
                value_free(step);
                value_free(end);

                runtime_t error = const_variable(var->name, &node->step.poss, &node->step.pose, context);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires_fail(error);
            }

regress:
            tmp = ires_merge(&ires, operate_success(var->value, step, poss, pose, context));
            if (IRES_HAS_ERROR(ires.response))
            {
                value_free(value);
                value_free(ires.value);
                value_free(step);
                value_free(end);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->body.nodes, node->body.size);
                    free(node->iterator);
                    free(node);
                }

                return ires;
            }

            var->value = tmp;

            value_free(ires.value);
            ires.value = value;
        }

    value_free(step);
    value_free(end);

    if (!IPROP_NOT_FREE(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        free(node->iterator);
        free(node);
    }

    ires.response &= LOOP_RESPONSE_MASK;
    return ires;
}

ires_t interpret_foreach(foreach_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            foreach_n_free(node);

        return ires_fail(invalid_access_statement("foreach statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value = NULL;

    value_p iterable = ires_merge(&ires, interpret_node(&node->iterable, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires;
    }

    if (!iterable)
    {
        runtime_t error = invalid_type("Iterable of foreach loop", "<str>, <list> or <tuple>", NONE_V,
            &node->iterable.poss, &node->iterable.pose, context);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }
    if (iterable->type != STR_V && iterable->type != LIST_V && iterable->type != TUPLE_V)
    {
        runtime_t error = invalid_type("Iterable of foreach loop", "<str>, <list> or <tuple>", iterable->type,
            &node->iterable.poss, &node->iterable.pose, context);

        value_free(iterable);

        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            free(node->iterator);
            free(node);
        }

        return ires_fail(error);
    }

    unsigned long long size = operate_size(iterable);

    char res = 0;
    value_p value;

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        value = operate_index(iterable, i);
        res = table_var_set(&context->table, 0, node->iterator, 0, value);
        if (res == -1)
        {
            value_free(ires.value);
            value_free(iterable);

            runtime_t error = const_variable(node->iterator, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }
        if (res)
        {
            value_free(ires.value);
            value_free(iterable);

            runtime_t error = type_specified_variable(node->iterator, res, poss, pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires_fail(error);
        }

        value = ires_merge(&ires, interpret_body(&node->body, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);
            value_free(iterable);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                free(node->iterator);
                free(node);
            }

            return ires;
        }

        if (IRES_FUNC_RETURN(ires.response))
        {
            value_free(ires.value);
            ires.value = value;

            break;
        }

        if (IRES_LOOP_CONTINUE(ires.response))
            continue;

        if (IRES_LOOP_BREAK(ires.response))
            break;

        value_free(ires.value);
        ires.value = value;
    }

    if (!IPROP_NOT_FREE(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        free(node->iterator);
        free(node);
    }

    ires.response &= LOOP_RESPONSE_MASK;
    return ires;
}

ires_t interpret_loop(loop_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            loop_n_free(node);

        return ires_fail(invalid_access_statement("loop statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value = NULL;

    value_p init = ires_merge(&ires, interpret_node(&node->init, context, properties & IPROP_MASK));
    if (IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->body.nodes, node->body.size);
            node_free(&node->step);
            node_free(&node->condition);
            free(node);
        }
        return ires;
    }

    value_free(init);

    value_p tmp, value;
    while (1)
    {
        tmp = ires_merge(&ires, interpret_node(&node->condition, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), IPROP_IN_LOOP(properties))));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->condition);
                free(node);
            }

            return ires;
        }

        if (!value_is_true(tmp))
            break;

        value_free(tmp);

        value = ires_merge(&ires, interpret_body(&node->body, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->condition);
                free(node);
            }

            return ires;
        }

        if (IRES_FUNC_RETURN(ires.response))
        {
            value_free(ires.value);
            ires.value = value;

            break;
        }

        if (IRES_LOOP_BREAK(ires.response))
            break;

        if (!IRES_LOOP_CONTINUE(ires.response))
        {
            value_free(ires.value);
            ires.value = value;
        }

        tmp = ires_merge(&ires, interpret_node(&node->step, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->step);
                node_free(&node->condition);
                free(node);
            }

            return ires;
        }

        value_free(tmp);
    }

    if (!IPROP_NOT_FREE(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        node_free(&node->step);
        node_free(&node->condition);
        free(node);
    }

    ires.response &= LOOP_RESPONSE_MASK;
    return ires;
}

ires_t interpret_do_while(do_while_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            do_while_n_free(node);

        return ires_fail(invalid_access_statement("do-while statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value = NULL;

    value_p condition, value;
    while (1)
    {
        value = ires_merge(&ires, interpret_body(&node->body, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }

            return ires;
        }

        if (IRES_FUNC_RETURN(ires.response))
        {
            value_free(ires.value);
            ires.value = value;

            break;
        }

        if (IRES_LOOP_BREAK(ires.response))
            break;

        if (!IRES_LOOP_CONTINUE(ires.response))
        {
            value_free(ires.value);
            ires.value = value;
        }

        condition = ires_merge(&ires, interpret_node(&node->condition, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), IPROP_IN_LOOP(properties))));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }
            return ires;
        }

        if (!value_is_true(condition))
        {
            value_free(condition);
            break;
        }

        value_free(condition);
    }

    if (!IPROP_NOT_FREE(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        node_free(&node->condition);
        free(node);
    }

    ires.response &= LOOP_RESPONSE_MASK;
    return ires;
}

ires_t interpret_while(while_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            while_n_free(node);

        return ires_fail(invalid_access_statement("while statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;
    ires.value = NULL;

    value_p condition, value;
    while (1)
    {
        condition = ires_merge(&ires, interpret_node(&node->condition, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), IPROP_IN_LOOP(properties))));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }

            return ires;
        }

        if (!value_is_true(condition))
        {
            value_free(condition);
            break;
        }

        value_free(condition);

        value = ires_merge(&ires, interpret_body(&node->body, context,
            IPROP_SET(0, 0, 1, IPROP_IN_FUNC(properties), 1)));
        if (IRES_HAS_ERROR(ires.response))
        {
            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->body.nodes, node->body.size);
                node_free(&node->condition);
                free(node);
            }

            return ires;
        }

        if (IRES_FUNC_RETURN(ires.response))
        {
            value_free(ires.value);
            ires.value = value;

            break;
        }

        if (IRES_LOOP_BREAK(ires.response))
            break;

        if (IRES_LOOP_CONTINUE(ires.response))
            continue;

        value_free(ires.value);
        ires.value = value;
    }

    if (!IPROP_NOT_FREE(properties))
    {
        node_p_free1(node->body.nodes, node->body.size);
        node_free(&node->condition);
        free(node);
    }

    ires.response &= LOOP_RESPONSE_MASK;
    return ires;
}

ires_t interpret_try(try_np node, pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            try_n_free(node);

        return ires_fail(invalid_access_statement("try statement", poss, pose, context));
    }

    ires_t ires;
    ires.response = 0;

    ires.value = ires_merge(&ires, interpret_body(&node->tbody, context, properties & IPROP_MASK));
    if (!IRES_HAS_ERROR(ires.response))
    {
        if (!IPROP_NOT_FREE(properties))
        {
            node_p_free1(node->fbody.nodes, node->fbody.size);
            case_p_free(node->excepts, node->size);
            free(node);
        }

        return ires;
    }

    runtime_t error = ires.error;
    ires.response = 0;

    char* label = NULL;
    unsigned long long code = 0;

    unsigned long long i;
    for (i = 0; i < node->size; i++)
    {
        ires.value = ires_merge(&ires, interpret_node(&node->excepts[i].condition, context, properties & IPROP_MASK));
        if (IRES_HAS_ERROR(ires.response))
        {
            context_free_debug(&error.context);
            free(error.detail);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->fbody.nodes, node->fbody.size);

                i++;
                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                    node_free(&node->excepts[node->size].condition);
                }
                i--;

                node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                free(node->excepts);

                free(node);
            }

            return ires;
        }

        if (!ires.value)
        {
            context_free_debug(&error.context);
            free(error.detail);

            error = invalid_type("Exception", "<int>, <bool>, <char> or <str>", NONE_V,
                &node->excepts[i].condition.poss, &node->excepts[i].condition.pose, context);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->fbody.nodes, node->fbody.size);

                i++;
                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                    node_free(&node->excepts[node->size].condition);
                }
                i--;

                node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                free(node->excepts);

                free(node);
            }

            return ires_fail(error);
        }

        switch (ires.value->type)
        {
        case INT_V:
            if (int_sign(ires.value->value.ptr) < 0 || !int_fits_ull(ires.value->value.ptr))
            {
                value_free_type(ires.value, int);

                context_free_debug(&error.context);
                free(error.detail);

                error = out_of_boundary("Error code", 0, ERROR_CODE_COUNT,
                    &node->excepts[i].condition.poss, &node->excepts[i].condition.pose, context);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    i++;
                    while (node->size > i)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    i--;

                    node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                    free(node->excepts);

                    free(node);
                }

                return ires_fail(error);
            }

            code = int_get_ull(ires.value->value.ptr);

            value_free_type(ires.value, int);
            break;
        case BOOL_V:
        case CHAR_V:
            code = ires.value->value.chr;

            value_free_shell(ires.value);
            break;
        case STR_V:
            label = str_str(ires.value->value.ptr);
            break;
        default:
            context_free_debug(&error.context);
            free(error.detail);

            error = invalid_type("Exception", "<int>, <bool>, <char> or <str>", ires.value->type,
                &node->excepts[i].condition.poss, &node->excepts[i].condition.pose, context);

            value_free(ires.value);

            if (!IPROP_NOT_FREE(properties))
            {
                node_p_free1(node->fbody.nodes, node->fbody.size);

                i++;
                while (node->size > i)
                {
                    node->size--;
                    node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                    node_free(&node->excepts[node->size].condition);
                }
                i--;

                node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                free(node->excepts);

                free(node);
            }

            return ires_fail(error);
        }

        if (!label)
        {
            if (code > ERROR_CODE_COUNT)
            {
                context_free_debug(&error.context);
                free(error.detail);

                error = out_of_boundary("Error code", 0, ERROR_CODE_COUNT,
                    &node->excepts[i].condition.poss, &node->excepts[i].condition.pose, context);

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    i++;
                    while (node->size > i)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    i--;

                    node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
                    free(node->excepts);

                    free(node);
                }

                return ires_fail(error);
            }

            if (code == ires.error.type)
            {
                context_free_debug(&error.context);
                free(error.detail);

                ires.value = ires_merge(&ires, interpret_body(&node->excepts[i].body, context, properties & IPROP_MASK));

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    i++;
                    while (node->size > i)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    i--;

                    free(node->excepts);
                    free(node);
                }

                return ires;
            }
        }
        else
        {
            if (!strcmp(label, error.detail) || !strcmp(label, runtime_labels[error.type]))
            {
                value_free_type(ires.value, str);

                context_free_debug(&error.context);
                free(error.detail);

                ires.value = ires_merge(&ires, interpret_body(&node->excepts[i].body, context, properties & IPROP_MASK));

                if (!IPROP_NOT_FREE(properties))
                {
                    node_p_free1(node->fbody.nodes, node->fbody.size);

                    i++;
                    while (node->size > i)
                    {
                        node->size--;
                        node_p_free1(node->excepts[node->size].body.nodes, node->excepts[node->size].body.size);
                        node_free(&node->excepts[node->size].condition);
                    }
                    i--;

                    free(node->excepts);
                    free(node);
                }

                return ires;
            }

            value_free_type(ires.value, str);
            label = NULL;
        }

        if (!IPROP_NOT_FREE(properties))
            node_p_free1(node->excepts[i].body.nodes, node->excepts[i].body.size);
    }

    if (!IPROP_NOT_FREE(properties))
        free(node->excepts);

    if (node->fbody.size)
    {
        context_free_debug(&error.context);
        free(error.detail);

        ires.value = ires_merge(&ires, interpret_body(&node->fbody, context, properties & IPROP_MASK));

        if (!IPROP_NOT_FREE(properties))
            free(node);

        return ires;
    }

    if (!IPROP_NOT_FREE(properties))
        free(node);

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
    if (IPROP_PTR(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            return_n_free(node);

        return ires_fail(invalid_access_statement("return statement", poss, pose, context));
    }

    if (!IPROP_IN_FUNC(properties))
    {
        if (!IPROP_NOT_FREE(properties))
            return_n_free(node);

        return ires_fail(outside_func(poss, pose, context));
    }

    ires_t ires;
    ires.response = IRES_RESPONSE_SET(0, 1, 0, 0, 0, 0);

    if (node)
    {
        ires.value = ires_merge(&ires, interpret_node(&node->value, context, properties));

        if (!IPROP_NOT_FREE(properties))
            free(node);
    }
    else
        ires.value = NULL;

    return ires;
}

ires_t interpret_continue(pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_statement("continue statement", poss, pose, context));

    if (!IPROP_IN_LOOP(properties))
        return ires_fail(outside_loop("Continue", poss, pose, context));

    ires_t ires;
    ires.response = IRES_RESPONSE_SET(0, 0, 1, 0, 0, 0);
    ires.value = NULL;

    return ires;
}

ires_t interpret_break(pos_p poss, pos_p pose, context_p context, char properties)
{
    if (IPROP_PTR(properties))
        return ires_fail(invalid_access_statement("break statement", poss, pose, context));

    if (!IPROP_IN_LOOP(properties))
        return ires_fail(outside_loop("Break", poss, pose, context));

    ires_t ires;
    ires.response = IRES_RESPONSE_SET(0, 0, 0, 1, 0, 0);
    ires.value = NULL;

    return ires;
}
