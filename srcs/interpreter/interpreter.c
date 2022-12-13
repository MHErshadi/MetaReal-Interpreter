/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <complex.h>
#include <str.h>
#include <interpreter/operation.h>
#include <stdlib.h>
#include <setting.h>
#include <lexer/token.h>

value_t ires_merge(ires_p ires, ires_t other);

ires_t interpret_node(node_p node, context_p context);
ires_t interpret_none(pos_p poss, pos_p pose, context_p context);
ires_t interpret_int(int_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_float(float_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_complex(complex_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_bool(char node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_char(char node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_str(str_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_tuple(list_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_dict(list_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_set(list_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_type(unsigned char node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_binary_operation(binary_operation_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_unary_operation(unary_operation_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_ternary_condition(ternary_condition_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_subscript(subscript_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_access(access_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_var_assign(var_assign_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_var_fixed_assign(var_fixed_assign_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_var_reassign(var_reassign_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_var_access(char* node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_func_def(func_def_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_func_call(func_call_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_class_def(class_def_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_struct_def(struct_def_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_dollar_func_call(dollar_func_call_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_if(if_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_switch(switch_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_for(for_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_foreach(foreach_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_loop(loop_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_do_while(do_while_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_while(while_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_try(try_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_import(char* node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_include(char* node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_return(return_np node, pos_p poss, pos_p pose, context_p context);
ires_t interpret_continue(pos_p poss, pos_p pose, context_p context);
ires_t interpret_break(pos_p poss, pos_p pose, context_p context);

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

value_t ires_merge(ires_p ires, ires_t other)
{
    if (other.has_error)
    {
        ires->error = other.error;
        ires->has_error = 1;
    }

    return other.value;
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
    case BINARY_OPERATION_N:
        return interpret_binary_operation(node->value.ptr, &node->poss, &node->pose, context);
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

ires_t interpret_list(list_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_tuple(list_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_dict(list_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_set(list_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_type(unsigned char node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_binary_operation(binary_operation_np node, pos_p poss, pos_p pose, context_p context)
{
    ires_t ires;
    ires.has_error = 0;

    value_t left = ires_merge(&ires, interpret_node(&node->left, context));
    if (ires.has_error)
    {
        node_free(&node->right);
        goto ret;
    }

    value_t right = ires_merge(&ires, interpret_node(&node->right, context));
    if (ires.has_error)
    {
        value_free(&left);
        goto ret;
    }

    switch (node->operator)
    {
    case PLUS_T:
        ires = operate_add(&left, &right);
        break;
    case MINUS_T:
        ires = operate_subtract(&left, &right);
        break;
    case MULTIPLY_T:
        ires = operate_multiply(&left, &right);
        break;
    case DIVIDE_T:
        ires = operate_divide(&left, &right);
        break;
    case MODULO_T:
        ires = operate_modulo(&left, &right);
        break;
    case QUOTIENT_T:
        ires = operate_quotient(&left, &right);
        break;
    case POWER_T:
        ires = operate_power(&left, &right);
        break;
    case B_AND_T:
        ires = operate_b_and(&left, &right);
        break;
    case B_OR_T:
        ires = operate_b_or(&left, &right);
        break;
    case B_XOR_T:
        ires = operate_b_xor(&left, &right);
        break;
    case LSHIFT_T:
        ires = operate_lshift(&left, &right);
        break;
    case RSHIFT_T:
        ires = operate_rshift(&left, &right);
        break;
    case EQUAL_T:
        ires = operate_equal(&left, &right);
        break;
    case NEQUAL_T:
        ires = operate_nequal(&left, &right);
        break;
    case LESS_T:
        ires = operate_less(&left, &right);
        break;
    case GREATER_T:
        ires = operate_greater(&left, &right);
        break;
    case LESS_EQ_T:
        ires = operate_less_eq(&left, &right);
        break;
    case GREATER_EQ_T:
        ires = operate_greater_eq(&left, &right);
        break;
    case AND_T:
    case AND_TK:
        ires = operate_greater(&left, &right);
        break;
    case OR_T:
    case OR_TK:
        ires = operate_less_eq(&left, &right);
        break;
    case XOR_T:
    case XOR_TK:
        ires = operate_greater_eq(&left, &right);
        break;
    }

    if (ires.has_error)
        goto ret;

    ires.value.poss = *poss;
    ires.value.pose = *pose;

ret:
    free(node);
    return ires;
}

ires_t interpret_unary_operation(unary_operation_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_ternary_condition(ternary_condition_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_subscript(subscript_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_access(access_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_var_assign(var_assign_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_var_fixed_assign(var_fixed_assign_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_var_reassign(var_reassign_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_var_access(char* node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_func_def(func_def_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_func_call(func_call_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_class_def(class_def_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_struct_def(struct_def_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_dollar_func_call(dollar_func_call_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_if(if_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_switch(switch_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_for(for_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_foreach(foreach_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_loop(loop_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_do_while(do_while_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_while(while_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_try(try_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_import(char* node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_include(char* node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_return(return_np node, pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_continue(pos_p poss, pos_p pose, context_p context)
{

}

ires_t interpret_break(pos_p poss, pos_p pose, context_p context)
{

}
