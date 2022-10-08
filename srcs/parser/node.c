/*/
 * MetaReal version 1.0.0
/*/

#include <parser/node.h>
#include <memory.h>
#include <lexer/token.h>

node_t node_set1(unsigned char type, void* value, pos_p poss, pos_p pose)
{
    node_t node;

    node.type = type;
    node.value = value;
    node.poss = *poss;
    node.pose = *pose;

    return node;
}

node_t node_set2(unsigned char type, pos_p poss, pos_p pose)
{
    node_t node;

    node.type = type;
    node.poss = *poss;
    node.pose = *pose;

    return node;
}

void node_print(FILE* stream, node_p node)
{
    if (node->type == NULL_N)
    {
        fputs("(NULL)", stream);
        return;
    }

    if (node->type == INT_N)
    {
        fprintf(stream, "(INT: %s)", ((int_np)node->value)->value);
        return;
    }
    if (node->type == FLOAT_N)
    {
        fprintf(stream, "(FLOAT: %s)", ((float_np)node->value)->value);
        return;
    }
    if (node->type == COMPLEX_N)
    {
        fprintf(stream, "(COMPLEX: %s)", ((complex_np)node->value)->value);
        return;
    }

    if (node->type == BINARY_OPERATION_N)
    {
        binary_operation_np value = node->value;

        fprintf(stream, "(BINARY_OPERATION: %s, ", token_labels[value->operator]);
        node_print(stream, &value->left);
        fputs(", ", stream);
        node_print(stream, &value->right);
        putc(')', stream);
        return;
    }
    if (node->type == UNARY_OPERATION_N)
    {
        unary_operation_np value = node->value;

        fprintf(stream, "(UNARY_OPERATION: %s, ", token_labels[value->operator]);
        node_print(stream, &value->operand);
        putc(')', stream);
        return;
    }
    if (node->type == TERNARY_CONDITION_N)
    {
        ternary_condition_np value = node->value;

        fputs("(TERNARY_CONDITION: ", stream);
        node_print(stream, &value->condition);
        fputs(", ", stream);
        node_print(stream, &value->left);
        fputs(", ", stream);
        node_print(stream, &value->right);
        putc(')', stream);
        return;
    }

    if (node->type == VAR_FIXED_ASSIGN_N)
    {
        var_fixed_assign_np value = node->value;

        fprintf(stream, "(VAR_FIXED_ASSIGN: %s, ", token_labels[value->operator]);
        node_print(stream, &value->var);
        putc(')', stream);
        return;
    }
}

pair_t pair_set(node_p key, node_p value)
{
    pair_t pair;

    pair.key = *key;
    pair.value = *value;

    return pair;
}

arg_t arg_set(const char* name, unsigned char type, node_p value)
{
    arg_t arg;

    arg.name = name;
    arg.type = type;
    arg.value = *value;

    return arg;
}

body_t body_set(node_p node)
{
    body_t body;

    body.nodes = stack_alloc(&memory.stack, sizeof(node_t));
    *body.nodes = *node;
    body.size = 1;

    return body;
}

arg_access_t arg_access_set(const char* name, node_p value)
{
    arg_access_t arg_access;

    arg_access.name = name;
    arg_access.value = *value;

    return arg_access;
}

case_t case_set(node_p condition, body_p body)
{
    case_t case_;

    case_.condition = *condition;
    case_.body = *body;

    return case_;
}

int_np int_n_set(const char* value, unsigned long long size)
{
    int_np int_n = stack_alloc(&memory.stack, sizeof(int_nt));

    int_n->value = value;
    int_n->size = size;

    return int_n;
}

float_np float_n_set(const char* value, unsigned long long size)
{
    float_np float_n = stack_alloc(&memory.stack, sizeof(float_nt));

    float_n->value = value;
    float_n->size = size;

    return float_n;
}

complex_np complex_n_set(const char* value, unsigned long long size)
{
    complex_np complex_n = stack_alloc(&memory.stack, sizeof(complex_nt));

    complex_n->value = value;
    complex_n->size = size;

    return complex_n;
}

str_np str_n_set(const char* value, unsigned long long size)
{
    str_np str_n = stack_alloc(&memory.stack, sizeof(str_nt));

    str_n->value = value;
    str_n->size = size;

    return str_n;
}

list_np list_n_set(node_p elements, unsigned long long size)
{
    list_np list_n = stack_alloc(&memory.stack, sizeof(list_nt));

    list_n->elements = elements;
    list_n->size = size;

    return list_n;
}

tuple_np tuple_n_set(node_p elements, unsigned long long size)
{
    tuple_np tuple_n = stack_alloc(&memory.stack, sizeof(tuple_nt));

    tuple_n->elements = elements;
    tuple_n->size = size;

    return tuple_n;
}

dict_np dict_n_set(pair_p elements, unsigned long long size)
{
    dict_np dict_n = stack_alloc(&memory.stack, sizeof(dict_nt));

    dict_n->elements = elements;
    dict_n->size = size;

    return dict_n;
}

set_np set_n_set(node_p elements, unsigned long long size)
{
    set_np set_n = stack_alloc(&memory.stack, sizeof(set_nt));

    set_n->elements = elements;
    set_n->size = size;

    return set_n;
}

binary_operation_np binary_operation_n_set(unsigned char operator, node_p left, node_p right)
{
    binary_operation_np binary_operation_n = stack_alloc(&memory.stack, sizeof(binary_operation_nt));

    binary_operation_n->operator = operator;
    binary_operation_n->left = *left;
    binary_operation_n->right = *right;

    return binary_operation_n;
}

unary_operation_np unary_operation_n_set(unsigned char operator, node_p operand)
{
    unary_operation_np unary_operation_n = stack_alloc(&memory.stack, sizeof(unary_operation_nt));

    unary_operation_n->operator = operator;
    unary_operation_n->operand = *operand;

    return unary_operation_n;
}

ternary_condition_np ternary_condition_n_set(node_p condition, node_p left, node_p right)
{
    ternary_condition_np ternary_condition_n = stack_alloc(&memory.stack, sizeof(ternary_condition_nt));

    ternary_condition_n->condition = *condition;
    ternary_condition_n->left = *left;
    ternary_condition_n->right = *right;

    return ternary_condition_n;
}

subscript_np subscript_n_set(node_p value, node_p pos)
{
    subscript_np subscript_n = stack_alloc(&memory.stack, sizeof(subscript_nt));

    subscript_n->value = *value;
    subscript_n->pos = *pos;

    return subscript_n;
}

access_np access_n_set(node_p value, node_p property)
{
    access_np access_n = stack_alloc(&memory.stack, sizeof(access_nt));

    access_n->value = *value;
    access_n->property = *property;

    return access_n;
}

var_assign_np var_assign_n_set(char properties, const char* name, unsigned char type, node_p value)
{
    var_assign_np var_assign_n = stack_alloc(&memory.stack, sizeof(var_assign_nt));

    var_assign_n->properties = properties;
    var_assign_n->name = name;
    var_assign_n->type = type;
    var_assign_n->value = *value;

    return var_assign_n;
}

var_fixed_assign_np var_fixed_assign_n_set(char properties, unsigned char operator, node_p var)
{
    var_fixed_assign_np var_fixed_assign_n = stack_alloc(&memory.stack, sizeof(var_fixed_assign_nt));

    var_fixed_assign_n->properties = properties;
    var_fixed_assign_n->operator = operator;
    var_fixed_assign_n->var = *var;

    return var_fixed_assign_n;
}

var_reassign_np var_reassign_n_set(unsigned char operator, node_p var, node_p value)
{
    var_reassign_np var_reassign_n = stack_alloc(&memory.stack, sizeof(var_reassign_nt));

    var_reassign_n->operator = operator;
    var_reassign_n->var = *var;
    var_reassign_n->value = *value;

    return var_reassign_n;
}

func_def_np func_def_n_set(char properties, const char* name, arg_p args, unsigned long long size, unsigned char type, body_p body)
{
    func_def_np func_def_n = stack_alloc(&memory.stack, sizeof(func_def_nt));

    func_def_n->properties = properties;
    func_def_n->name = name;
    func_def_n->args = args;
    func_def_n->size = size;
    func_def_n->type = type;
    func_def_n->body = *body;

    return func_def_n;
}

func_call_np func_call_n_set(node_p func, arg_access_p args, unsigned long long size)
{
    func_call_np func_call_n = stack_alloc(&memory.stack, sizeof(func_call_nt));

    func_call_n->func = *func;
    func_call_n->args = args;
    func_call_n->size = size;

    return func_call_n;
}

class_def_np class_def_n_set(char properties, const char* name, body_p body)
{
    class_def_np class_def_n = stack_alloc(&memory.stack, sizeof(class_def_nt));

    class_def_n->properties = properties;
    class_def_n->name = name;
    class_def_n->body = *body;

    return class_def_n;
}

struct_def_np struct_def_n_set(char properties, const char* name, body_p body)
{
    struct_def_np struct_def_n = stack_alloc(&memory.stack, sizeof(struct_def_nt));

    struct_def_n->properties = properties;
    struct_def_n->name = name;
    struct_def_n->body = *body;

    return struct_def_n;
}

enum_def_np enum_def_n_set(char properties, const char* name, const char** elements, unsigned long long size)
{
    enum_def_np enum_def_n = stack_alloc(&memory.stack, sizeof(enum_def_nt));

    enum_def_n->properties = properties;
    enum_def_n->name = name;
    enum_def_n->elements = elements;
    enum_def_n->size = size;

    return enum_def_n;
}

dollar_func_call_np dollar_func_call_n_set(const char* name, node_p args, unsigned long long size)
{
    dollar_func_call_np dollar_func_call_n = stack_alloc(&memory.stack, sizeof(dollar_func_call_nt));

    dollar_func_call_n->name = name;
    dollar_func_call_n->args = args;
    dollar_func_call_n->size = size;

    return dollar_func_call_n;
}

if_np if_n_set(case_p cases, unsigned long long size, body_p ebody)
{
    if_np if_n = stack_alloc(&memory.stack, sizeof(if_nt));

    if_n->cases = cases;
    if_n->size = size;
    if_n->ebody = *ebody;

    return if_n;
}

switch_np switch_n_set(node_p value, case_p cases, unsigned long long size, body_p dbody)
{
    switch_np switch_n = stack_alloc(&memory.stack, sizeof(switch_nt));

    switch_n->value = *value;
    switch_n->cases = cases;
    switch_n->size = size;
    switch_n->dbody = *dbody;

    return switch_n;
}

for_np for_n_set(const char* iterator, node_p start, node_p end, node_p step, body_p body)
{
    for_np for_n = stack_alloc(&memory.stack, sizeof(for_nt));

    for_n->iterator = iterator;
    for_n->start = *start;
    for_n->end = *end;
    for_n->step = *step;
    for_n->body = *body;

    return for_n;
}

foreach_np foreach_n_set(const char* iterator, node_p iterable, body_p body)
{
    foreach_np foreach_n = stack_alloc(&memory.stack, sizeof(foreach_nt));

    foreach_n->iterator = iterator;
    foreach_n->iterable = *iterable;
    foreach_n->body = *body;

    return foreach_n;
}

loop_np loop_n_set(node_p init, node_p condition, node_p step, body_p body)
{
    loop_np loop_n = stack_alloc(&memory.stack, sizeof(loop_nt));

    loop_n->init = *init;
    loop_n->condition = *condition;
    loop_n->step = *step;
    loop_n->body = *body;

    return loop_n;
}

do_while_np do_while_n_set(body_p body, node_p condition)
{
    do_while_np do_while_n = stack_alloc(&memory.stack, sizeof(do_while_nt));

    do_while_n->body = *body;
    do_while_n->condition = *condition;

    return do_while_n;
}

while_np while_n_set(node_p condition, body_p body)
{
    while_np while_n = stack_alloc(&memory.stack, sizeof(while_nt));

    while_n->condition = *condition;
    while_n->body = *body;

    return while_n;
}

try_np try_n_set(body_p tbody, case_p excepts, unsigned long long size, body_p fbody)
{
    try_np try_n = stack_alloc(&memory.stack, sizeof(try_nt));

    try_n->tbody = *tbody;
    try_n->excepts = excepts;
    try_n->size = size;
    try_n->fbody = *fbody;

    return try_n;
}

return_np return_n_set(node_p value)
{
    return_np return_n = stack_alloc(&memory.stack, sizeof(return_nt));

    return_n->value = *value;

    return return_n;
}
