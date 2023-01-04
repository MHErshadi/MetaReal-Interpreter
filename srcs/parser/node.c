/*/
 * MetaReal version 1.0.0
/*/

#include <parser/node.h>
#include <lexer/token.h>
#include <stdlib.h>
#include <string.h>

void body_print(FILE* stream, body_p body);

void node_p_print(FILE* stream, node_p nodes, unsigned long long size);
void pair_p_print(FILE* stream, pair_p pairs, unsigned long long size);
void arg_p_print(FILE* stream, arg_p args, unsigned long long size);
void arg_access_p_print(FILE* stream, arg_access_p args, unsigned long long size);
void case_p_print(FILE* stream, case_p cases, unsigned long long size);

node_t node_set1(unsigned char type, void* value, pos_p poss, pos_p pose)
{
    node_t node;

    node.type = type;
    node.value.ptr = value;
    node.poss = *poss;
    node.pose = *pose;

    return node;
}

node_t node_set2(unsigned char type, unsigned char value, pos_p poss, pos_p pose)
{
    node_t node;

    node.type = type;
    node.value.chr = value;
    node.poss = *poss;
    node.pose = *pose;

    return node;
}

node_t node_set3(unsigned char type, pos_p poss, pos_p pose)
{
    node_t node;

    node.type = type;
    node.poss = *poss;
    node.pose = *pose;

    return node;
}

node_t node_copy(const node_p node)
{
    node_t copy = *node;

    switch (node->type)
    {
    case NULL_N:
    case NONE_N:
    case BOOL_N:
    case CHAR_N:
    case TYPE_N:
    case CONTINUE_N:
    case BREAK_N:
        return copy;
    case VAR_ACCESS_N:
    case IMPORT_N:
    case INCLUDE_N:
        strcpy(copy.value.ptr, copy.value.ptr);
        return copy;
    case INT_N:
        copy.value.ptr = int_n_copy(copy.value.ptr);
        return copy;
    case FLOAT_N:
        copy.value.ptr = float_n_copy(copy.value.ptr);
        return copy;
    case COMPLEX_N:
        copy.value.ptr = complex_n_copy(copy.value.ptr);
        return copy;
    case STR_N:
        copy.value.ptr = str_n_copy(copy.value.ptr);
        return copy;
    case LIST_N:
        copy.value.ptr = list_n_copy(copy.value.ptr);
        return copy;
    case TUPLE_N:
        copy.value.ptr = tuple_n_copy(copy.value.ptr);
        return copy;
    case DICT_N:
        copy.value.ptr = dict_n_copy(copy.value.ptr);
        return copy;
    case SET_N:
        copy.value.ptr = set_n_copy(copy.value.ptr);
        return copy;
    case BINARY_OPERATION_N:
        copy.value.ptr = binary_operation_n_copy(copy.value.ptr);
        return copy;
    case UNARY_OPERATION_N:
        copy.value.ptr = unary_operation_n_copy(copy.value.ptr);
        return copy;
    case TERNARY_CONDITION_N:
        copy.value.ptr = ternary_condition_n_copy(copy.value.ptr);
        return copy;
    case SUBSCRIPT_N:
        copy.value.ptr = subscript_n_copy(copy.value.ptr);
        return copy;
    case ACCESS_N:
        copy.value.ptr = access_n_copy(copy.value.ptr);
        return copy;
    case VAR_ASSIGN_N:
        copy.value.ptr = var_assign_n_copy(copy.value.ptr);
        return copy;
    case VAR_FIXED_ASSIGN_N:
        copy.value.ptr = var_fixed_assign_n_copy(copy.value.ptr);
        return copy;
    case VAR_REASSIGN_N:
        copy.value.ptr = var_reassign_n_copy(copy.value.ptr);
        return copy;
    case FUNC_DEF_N:
        copy.value.ptr = func_def_n_copy(copy.value.ptr);
        return copy;
    case FUNC_CALL_N:
        copy.value.ptr = func_call_n_copy(copy.value.ptr);
        return copy;
    case CLASS_DEF_N:
        copy.value.ptr = class_def_n_copy(copy.value.ptr);
        return copy;
    case STRUCT_DEF_N:
        copy.value.ptr = struct_def_n_copy(copy.value.ptr);
        return copy;
    case DOLLAR_FUNC_CALL_N:
        copy.value.ptr = dollar_func_call_n_copy(copy.value.ptr);
        return copy;
    case IF_N:
        copy.value.ptr = if_n_copy(copy.value.ptr);
        return copy;
    case SWITCH_N:
        copy.value.ptr = switch_n_copy(copy.value.ptr);
        return copy;
    case FOR_N:
        copy.value.ptr = for_n_copy(copy.value.ptr);
        return copy;
    case FOREACH_N:
        copy.value.ptr = foreach_n_copy(copy.value.ptr);
        return copy;
    case LOOP_N:
        copy.value.ptr = loop_n_copy(copy.value.ptr);
        return copy;
    case DO_WHILE_N:
        copy.value.ptr = do_while_n_copy(copy.value.ptr);
        return copy;
    case WHILE_N:
        copy.value.ptr = while_n_copy(copy.value.ptr);
        return copy;
    case TRY_N:
        copy.value.ptr = try_n_copy(copy.value.ptr);
        return copy;
    case RETURN_N:
        copy.value.ptr = return_n_copy(copy.value.ptr);
        break;
    }

    return copy;
}

void node_free(node_p node)
{
    switch (node->type)
    {
    case NULL_N:
    case NONE_N:
    case BOOL_N:
    case CHAR_N:
    case TYPE_N:
    case CONTINUE_N:
    case BREAK_N:
        return;
    case VAR_ACCESS_N:
    case IMPORT_N:
    case INCLUDE_N:
        free(node->value.ptr);
        return;
    case INT_N:
    case FLOAT_N:
    case COMPLEX_N:
    case STR_N:
        free(node->value.ptr);
        return;
    case LIST_N:
        if (node->value.ptr)
            list_n_free(node->value.ptr);
        return;
    case TUPLE_N:
        tuple_n_free(node->value.ptr);
        return;
    case DICT_N:
        if (node->value.ptr)
            dict_n_free(node->value.ptr);
        return;
    case SET_N:
        set_n_free(node->value.ptr);
        return;
    case BINARY_OPERATION_N:
        binary_operation_n_free(node->value.ptr);
        return;
    case UNARY_OPERATION_N:
        unary_operation_n_free(node->value.ptr);
        return;
    case TERNARY_CONDITION_N:
        ternary_condition_n_free(node->value.ptr);
        return;
    case SUBSCRIPT_N:
        subscript_n_free(node->value.ptr);
        return;
    case ACCESS_N:
        access_n_free(node->value.ptr);
        return;
    case VAR_ASSIGN_N:
        var_assign_n_free(node->value.ptr);
        return;
    case VAR_FIXED_ASSIGN_N:
        var_fixed_assign_n_free(node->value.ptr);
        return;
    case VAR_REASSIGN_N:
        var_reassign_n_free(node->value.ptr);
        return;
    case FUNC_DEF_N:
        func_def_n_free(node->value.ptr);
        return;
    case FUNC_CALL_N:
        func_call_n_free(node->value.ptr);
        return;
    case CLASS_DEF_N:
        class_def_n_free(node->value.ptr);
        return;
    case STRUCT_DEF_N:
        struct_def_n_free(node->value.ptr);
        return;
    case DOLLAR_FUNC_CALL_N:
        dollar_func_call_n_free(node->value.ptr);
        return;
    case IF_N:
        if_n_free(node->value.ptr);
        return;
    case SWITCH_N:
        switch_n_free(node->value.ptr);
        return;
    case FOR_N:
        for_n_free(node->value.ptr);
        return;
    case FOREACH_N:
        foreach_n_free(node->value.ptr);
        return;
    case LOOP_N:
        loop_n_free(node->value.ptr);
        return;
    case DO_WHILE_N:
        do_while_n_free(node->value.ptr);
        return;
    case WHILE_N:
        while_n_free(node->value.ptr);
        return;
    case TRY_N:
        try_n_free(node->value.ptr);
        return;
    case RETURN_N:
        return_n_free(node->value.ptr);
        return;
    }
}

void node_print(FILE* stream, node_p node)
{
    if (node->type == NULL_N)
    {
        fputs("(NULL)", stream);
        return;
    }

    if (node->type == NONE_N)
    {
        fputs("(NONE)", stream);
        return;
    }

    if (node->type == INT_N)
    {
        fprintf(stream, "(INT: %s)", ((int_np)node->value.ptr)->value);
        return;
    }
    if (node->type == FLOAT_N)
    {
        fprintf(stream, "(FLOAT: %s)", ((float_np)node->value.ptr)->value);
        return;
    }
    if (node->type == COMPLEX_N)
    {
        fprintf(stream, "(COMPLEX: %s)", ((complex_np)node->value.ptr)->value);
        return;
    }

    if (node->type == BOOL_N)
    {
        fprintf(stream, "(BOOL: %u)", node->value.chr);
        return;
    }

    if (node->type == CHAR_N)
    {
        fputs("(CHAR: '", stream);

        switch (node->value.chr)
        {
        case '\0':
            fputs("\\0')", stream);
            return;
        case '\a':
            fputs("\\a')", stream);
            return;
        case '\b':
            fputs("\\b')", stream);
            return;
        case '\f':
            fputs("\\f')", stream);
            return;
        case '\n':
            fputs("\\n')", stream);
            return;
        case '\r':
            fputs("\\r')", stream);
            return;
        case '\t':
            fputs("\\t')", stream);
            return;
        case '\v':
            fputs("\\v')", stream);
            return;
        default:
            fprintf(stream, "%c')", node->value.chr);
            return;
        }
    }

    if (node->type == STR_N)
    {
        str_np value = node->value.ptr;

        fputs("(STR: \"", stream);

        unsigned long long i;
        for (i = 0; i < value->size; i++)
            switch (value->value[i])
            {
            case '\0':
                fputs("\\0", stream);
                break;
            case '\a':
                fputs("\\a", stream);
                break;
            case '\b':
                fputs("\\b", stream);
                break;
            case '\f':
                fputs("\\f", stream);
                break;
            case '\n':
                fputs("\\n", stream);
                break;
            case '\r':
                fputs("\\r", stream);
                break;
            case '\t':
                fputs("\\t", stream);
                break;
            case '\v':
                fputs("\\v", stream);
                break;
            default:
                putc(value->value[i], stream);
                break;
            }

        fputs("\")", stream);
        return;
    }

    if (node->type == LIST_N)
    {
        list_np value = node->value.ptr;

        if (!node->value.ptr)
        {
            fputs("(LIST)", stream);
            return;
        }

        fputs("(LIST: {", stream);
        node_p_print(stream, value->elements, value->size);
        fputs("})", stream);
        return;
    }
    if (node->type == TUPLE_N)
    {
        tuple_np value = node->value.ptr;

        fputs("(TUPLE: {", stream);
        node_p_print(stream, value->elements, value->size);
        fputs("})", stream);
        return;
    }
    if (node->type == DICT_N)
    {
        dict_np value = node->value.ptr;

        if (!node->value.ptr)
        {
            fputs("(DICT)", stream);
            return;
        }

        fputs("(DICT: {", stream);
        pair_p_print(stream, value->elements, value->size);
        fputs("})", stream);
    }
    if (node->type == SET_N)
    {
        set_np value = node->value.ptr;

        fputs("(SET: {", stream);
        node_p_print(stream, value->elements, value->size);
        fputs("})", stream);
        return;
    }

    if (node->type == TYPE_N)
    {
        fprintf(stream, "(TYPE: %s)", token_labels[node->value.chr]);
        return;
    }

    if (node->type == BINARY_OPERATION_N)
    {
        binary_operation_np value = node->value.ptr;

        fprintf(stream, "(BINARY_OPERATION: %s, ", token_labels[value->operator]);
        node_print(stream, &value->left);
        fputs(", ", stream);
        node_print(stream, &value->right);
        putc(')', stream);
        return;
    }
    if (node->type == UNARY_OPERATION_N)
    {
        unary_operation_np value = node->value.ptr;

        fprintf(stream, "(UNARY_OPERATION: %s, ", token_labels[value->operator]);
        node_print(stream, &value->operand);
        putc(')', stream);
        return;
    }

    if (node->type == TERNARY_CONDITION_N)
    {
        ternary_condition_np value = node->value.ptr;

        fputs("(TERNARY_CONDITION: ", stream);
        node_print(stream, &value->condition);
        fputs(", ", stream);
        node_print(stream, &value->left);
        fputs(", ", stream);
        node_print(stream, &value->right);
        putc(')', stream);
        return;
    }

    if (node->type == SUBSCRIPT_N)
    {
        subscript_np value = node->value.ptr;

        fputs("(SUBSCRIPT: ", stream);
        node_print(stream, &value->value);
        fputs(", ", stream);
        node_print(stream, &value->pos);
        putc(')', stream);
        return;
    }

    if (node->type == ACCESS_N)
    {
        access_np value = node->value.ptr;

        fputs("(ACCESS: ", stream);
        node_print(stream, &value->value);
        fputs(", ", stream);
        node_print(stream, &value->property);
        putc(')', stream);
        return;
    }

    if (node->type == VAR_ASSIGN_N)
    {
        var_assign_np value = node->value.ptr;

        fprintf(stream, "(VAR_ASSIGN: (#public=%u, #global=%u, #const=%u, #static=%u), %s, %s, ",
            PROP_PUBLIC(value->properties), PROP_GLOBAL(value->properties), PROP_CONST(value->properties), PROP_STATIC(value->properties),
            value->name, token_labels[value->type]);
        node_print(stream, &value->value);
        putc(')', stream);
        return;
    }
    if (node->type == VAR_FIXED_ASSIGN_N)
    {
        var_fixed_assign_np value = node->value.ptr;

        fprintf(stream, "(VAR_FIXED_ASSIGN: (#post=%u), %s, ",
            value->properties,
            token_labels[value->operator]);
        node_print(stream, &value->var);
        putc(')', stream);
        return;
    }
    if (node->type == VAR_REASSIGN_N)
    {
        var_reassign_np value = node->value.ptr;

        fprintf(stream, "(VAR_REASSIGN: %s, ", token_labels[value->operator]);
        node_print(stream, &value->var);
        fputs(", ", stream);
        node_print(stream, &value->value);
        putc(')', stream);
        return;
    }
    if (node->type == VAR_ACCESS_N)
    {
        fprintf(stream, "(VAR_ACCESS: %s)", node->value);
        return;
    }

    if (node->type == FUNC_DEF_N)
    {
        func_def_np value = node->value.ptr;

        fprintf(stream, "(FUNC_DEF: (#public=%u, #global=%u, #const=%u, #static=%u), %s, %s, {",
            PROP_PUBLIC(value->properties), PROP_GLOBAL(value->properties), PROP_CONST(value->properties), PROP_STATIC(value->properties),
            value->name, token_labels[value->type]);
        arg_p_print(stream, value->args, value->size);
        fputs("}, ", stream);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }
    if (node->type == FUNC_CALL_N)
    {
        func_call_np value = node->value.ptr;

        fputs("(FUNC_CALL: ", stream);
        node_print(stream, &value->func);
        fputs(", {", stream);
        arg_access_p_print(stream, value->args, value->size);
        fputs("})", stream);
        return;
    }

    if (node->type == CLASS_DEF_N)
    {
        class_def_np value = node->value.ptr;

        fprintf(stream, "(CLASS_DEF: (#public=%u, #global=%u, #const=%u, #static=%u), %s, ",
            PROP_PUBLIC(value->properties), PROP_GLOBAL(value->properties), PROP_CONST(value->properties), PROP_STATIC(value->properties),
            value->name);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }

    if (node->type == STRUCT_DEF_N)
    {
        struct_def_np value = node->value.ptr;

        fprintf(stream, "(STRUCT_DEF: (#public=%u, #global=%u, #const=%u, #static=%u), %s, ",
            PROP_PUBLIC(value->properties), PROP_GLOBAL(value->properties), PROP_CONST(value->properties), PROP_STATIC(value->properties),
            value->name);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }

    if (node->type == DOLLAR_FUNC_CALL_N)
    {
        dollar_func_call_np value = node->value.ptr;

        fprintf(stream, "(DOLLAR_FUNC_CALL: %s, {", value->name);
        node_p_print(stream, value->args, value->size);
        fputs("})", stream);
        return;
    }

    if (node->type == IF_N)
    {
        if_np value = node->value.ptr;

        fputs("(IF: {", stream);
        case_p_print(stream, value->cases, value->size);
        fputs("}, ", stream);
        body_print(stream, &value->ebody);
        fputc(')', stream);
        return;
    }
    if (node->type == SWITCH_N)
    {
        switch_np value = node->value.ptr;

        fputs("(SWITCH: ", stream);
        node_print(stream, &value->value);
        fputs(", {", stream);
        case_p_print(stream, value->cases, value->size);
        fputs("}, ", stream);
        body_print(stream, &value->dbody);
        fputc(')', stream);
        return;
    }

    if (node->type == FOR_N)
    {
        for_np value = node->value.ptr;

        fprintf(stream, "(FOR: %s, ", value->iterator);
        node_print(stream, &value->start);
        fputs(", ", stream);
        node_print(stream, &value->end);
        fputs(", ", stream);
        node_print(stream, &value->step);
        fputs(", ", stream);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }
    if (node->type == FOREACH_N)
    {
        foreach_np value = node->value.ptr;

        fprintf(stream, "(FOREACH: %s, ", value->iterator);
        node_print(stream, &value->iterable);
        fputs(", ", stream);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }
    if (node->type == LOOP_N)
    {
        loop_np value = node->value.ptr;

        fputs("(LOOP: ", stream);
        node_print(stream, &value->init);
        fputs(", ", stream);
        node_print(stream, &value->condition);
        fputs(", ", stream);
        node_print(stream, &value->step);
        fputs(", ", stream);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }

    if (node->type == WHILE_N)
    {
        while_np value = node->value.ptr;

        fputs("(WHILE: ", stream);
        node_print(stream, &value->condition);
        fputs(", ", stream);
        body_print(stream, &value->body);
        fputc(')', stream);
        return;
    }
    if (node->type == DO_WHILE_N)
    {
        do_while_np value = node->value.ptr;

        fputs("(DO_WHILE: ", stream);
        body_print(stream, &value->body);
        fputs(", ", stream);
        node_print(stream, &value->condition);
        fputc(')', stream);
        return;
    }

    if (node->type == TRY_N)
    {
        try_np value = node->value.ptr;

        fputs("(TRY: ", stream);
        body_print(stream, &value->tbody);
        fputs(", {", stream);
        case_p_print(stream, value->excepts, value->size);
        fputs("}, ", stream);
        body_print(stream, &value->fbody);
        fputc(')', stream);
        return;
    }

    if (node->type == IMPORT_N)
    {
        fprintf(stream, "(IMPORT: %s)", node->value);
        return;
    }
    if (node->type == INCLUDE_N)
    {
        fprintf(stream, "(INCLUDE: %s)", node->value);
        return;
    }

    if (node->type == RETURN_N)
    {
        if (!node->value.ptr)
        {
            fputs("(RETURN)", stream);
            return;
        }

        fputs("(RETURN: ", stream);
        node_print(stream, &((return_np)node->value.ptr)->value);
        putc(')', stream);
        return;
    }

    if (node->type == CONTINUE_N)
    {
        fputs("(CONTINUE)", stream);
        return;
    }
    if (node->type == BREAK_N)
    {
        fputs("(BREAK)", stream);
        return;
    }
}

node_p node_p_copy(const node_p nodes, unsigned long long size)
{
    if (!size)
        return NULL;

    node_p copy = malloc(size * sizeof(node_t));

    unsigned long long i;
    for (i = 0; i < size; i++)
        copy[i] = node_copy(nodes + i);

    return copy;
}

void node_p_free1(node_p nodes, unsigned long long size)
{
    if (!size)
        return;

    while (size)
        node_free(nodes + --size);
    free(nodes);
}

void node_p_free2(node_p nodes)
{
    while (nodes->type != NULL_N)
        node_free(nodes++);
}

body_t body_set(node_p node)
{
    body_t body;

    body.nodes = malloc(sizeof(node_t));
    *body.nodes = *node;
    body.size = 1;

    return body;
}

pair_p pair_p_copy(const pair_p pairs, unsigned long long size)
{
    if (!size)
        return NULL;

    pair_p copy = malloc(size * sizeof(pair_t));

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        copy[i].key = node_copy(&pairs[i].key);
        copy[i].value = node_copy(&pairs[i].value);
    }

    return copy;
}

void pair_p_free(pair_p pairs, unsigned long long size)
{
    if (!size)
        return;

    while (size)
    {
        node_free(&pairs[--size].value);
        node_free(&pairs[size].key);
    }
    free(pairs);
}

arg_p arg_p_copy(const arg_p args, unsigned long long size)
{
    if (!size)
        return NULL;

    arg_p copy = malloc(size * sizeof(arg_t));

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        strcpy(copy[i].name, args[i].name);

        copy[i].type = args[i].type;
        copy[i].value = node_copy(&args[i].value);
    }

    return copy;
}

void arg_p_free(arg_p args, unsigned long long size)
{
    if (!size)
        return;

    while (size)
        node_free(&args[--size].value);
    free(args);
}

arg_access_p arg_access_p_copy(const arg_access_p args, unsigned long long size)
{
    if (!size)
        return NULL;

    arg_access_p copy = malloc(size * sizeof(arg_access_t));

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        strcpy(copy[i].name, args[i].name);

        copy[i].value = node_copy(&args[i].value);
    }

    return copy;
}

void arg_access_p_free(arg_access_p args, unsigned long long size)
{
    if (!size)
        return;

    while (size)
        node_free(&args[--size].value);
    free(args);
}

case_p case_p_copy(const case_p cases, unsigned long long size)
{
    if (!size)
        return NULL;

    case_p copy = malloc(size * sizeof(case_t));

    unsigned long long i;
    for (i = 0; i < size; i++)
    {
        copy[i].condition = node_copy(&cases[i].condition);

        copy[i].body.nodes = node_p_copy(cases[i].body.nodes, cases[i].body.size);
        copy[i].body.size = cases[i].body.size;
    }

    return copy;
}

void case_p_free(case_p cases, unsigned long long size)
{
    if (!size)
        return;

    while (size)
    {
        size--;

        node_p_free1(cases[size].body.nodes, cases[size].body.size);
        node_free(&cases[size].condition);
    }
    free(cases);
}

int_np int_n_set(char* value, unsigned long long size)
{
    int_np int_n = malloc(sizeof(int_nt));

    int_n->value = value;
    int_n->size = size;

    return int_n;
}

int_np int_n_copy(const int_np node)
{
    int_np copy = malloc(sizeof(int_nt));

    copy->value = malloc(node->size + 1);
    strcpy(copy->value, node->value);

    copy->size = node->size;

    return copy;
}

void int_n_free(int_np node)
{
    free(node->value);
    free(node);
}

float_np float_n_set(char* value, unsigned long long size)
{
    float_np float_n = malloc(sizeof(float_nt));

    float_n->value = value;
    float_n->size = size;

    return float_n;
}

float_np float_n_copy(const float_np node)
{
    float_np copy = malloc(sizeof(float_nt));

    copy->value = malloc(node->size + 1);
    strcpy(copy->value, node->value);

    copy->size = node->size;

    return copy;
}

void float_n_free(float_np node)
{
    free(node->value);
    free(node);
}

complex_np complex_n_set(char* value, unsigned long long size)
{
    complex_np complex_n = malloc(sizeof(complex_nt));

    complex_n->value = value;
    complex_n->size = size;

    return complex_n;
}

complex_np complex_n_copy(const complex_np node)
{
    complex_np copy = malloc(sizeof(complex_nt));

    copy->value = malloc(node->size + 1);
    strcpy(copy->value, node->value);

    copy->size = node->size;

    return copy;
}

void complex_n_free(complex_np node)
{
    free(node->value);
    free(node);
}

str_np str_n_set(char* value, unsigned long long size)
{
    str_np str_n = malloc(sizeof(str_nt));

    str_n->value = value;
    str_n->size = size;

    return str_n;
}

str_np str_n_copy(const str_np node)
{
    str_np copy = malloc(sizeof(str_nt));

    copy->value = malloc(node->size + 1);
    strcpy(copy->value, node->value);

    copy->size = node->size;

    return copy;
}

void str_n_free(str_np node)
{
    free(node->value);
    free(node);
}

list_np list_n_set(node_p elements, unsigned long long size)
{
    list_np list_n = malloc(sizeof(list_nt));

    list_n->elements = elements;
    list_n->size = size;

    return list_n;
}

list_np list_n_copy(const list_np node)
{
    list_np copy = malloc(sizeof(list_nt));

    copy->elements = node_p_copy(node->elements, node->size);
    copy->size = node->size;

    return copy;
}

void list_n_free(list_np node)
{
    node_p_free1(node->elements, node->size);
    free(node);
}

tuple_np tuple_n_set(node_p elements, unsigned long long size)
{
    tuple_np tuple_n = malloc(sizeof(tuple_nt));

    tuple_n->elements = elements;
    tuple_n->size = size;

    return tuple_n;
}

tuple_np tuple_n_copy(const tuple_np node)
{
    tuple_np copy = malloc(sizeof(tuple_nt));

    copy->elements = node_p_copy(node->elements, node->size);
    copy->size = node->size;

    return copy;
}

void tuple_n_free(tuple_np node)
{
    node_p_free1(node->elements, node->size);
    free(node);
}

dict_np dict_n_set(pair_p elements, unsigned long long size)
{
    dict_np dict_n = malloc(sizeof(dict_nt));

    dict_n->elements = elements;
    dict_n->size = size;

    return dict_n;
}

dict_np dict_n_copy(const dict_np node)
{
    dict_np copy = malloc(sizeof(dict_nt));

    copy->elements = pair_p_copy(node->elements, node->size);
    copy->size = node->size;

    return copy;
}

void dict_n_free(dict_np node)
{
    pair_p_free(node->elements, node->size);
    free(node);
}

set_np set_n_set(node_p elements, unsigned long long size)
{
    set_np set_n = malloc(sizeof(set_nt));

    set_n->elements = elements;
    set_n->size = size;

    return set_n;
}

set_np set_n_copy(const set_np node)
{
    set_np copy = malloc(sizeof(set_nt));

    copy->elements = node_p_copy(node->elements, node->size);
    copy->size = node->size;

    return copy;
}

void set_n_free(set_np node)
{
    node_p_free1(node->elements, node->size);
    free(node);
}

binary_operation_np binary_operation_n_set(unsigned char operator, node_p left, node_p right)
{
    binary_operation_np binary_operation_n = malloc(sizeof(binary_operation_nt));

    binary_operation_n->operator = operator;
    binary_operation_n->left = *left;
    binary_operation_n->right = *right;

    return binary_operation_n;
}

binary_operation_np binary_operation_n_copy(const binary_operation_np node)
{
    binary_operation_np copy = malloc(sizeof(binary_operation_nt));

    copy->operator = node->operator;

    copy->left = node_copy(&node->left);
    copy->right = node_copy(&node->right);

    return copy;
}

void binary_operation_n_free(binary_operation_np node)
{
    node_free(&node->right);
    node_free(&node->left);
    free(node);
}

unary_operation_np unary_operation_n_set(unsigned char operator, node_p operand)
{
    unary_operation_np unary_operation_n = malloc(sizeof(unary_operation_nt));

    unary_operation_n->operator = operator;
    unary_operation_n->operand = *operand;

    return unary_operation_n;
}

unary_operation_np unary_operation_n_copy(const unary_operation_np node)
{
    unary_operation_np copy = malloc(sizeof(unary_operation_nt));

    copy->operator = node->operator;

    copy->operand = node_copy(&node->operand);

    return copy;
}

void unary_operation_n_free(unary_operation_np node)
{
    node_free(&node->operand);
    free(node);
}

ternary_condition_np ternary_condition_n_set(node_p condition, node_p left, node_p right)
{
    ternary_condition_np ternary_condition_n = malloc(sizeof(ternary_condition_nt));

    ternary_condition_n->condition = *condition;
    ternary_condition_n->left = *left;
    ternary_condition_n->right = *right;

    return ternary_condition_n;
}

ternary_condition_np ternary_condition_n_copy(const ternary_condition_np node)
{
    ternary_condition_np copy = malloc(sizeof(ternary_condition_nt));

    copy->condition = node_copy(&node->condition);

    copy->left = node_copy(&node->left);
    copy->right = node_copy(&node->right);

    return copy;
}

void ternary_condition_n_free(ternary_condition_np node)
{
    node_free(&node->right);
    node_free(&node->left);
    node_free(&node->condition);
    free(node);
}

subscript_np subscript_n_set(node_p value, node_p pos)
{
    subscript_np subscript_n = malloc(sizeof(subscript_nt));

    subscript_n->value = *value;
    subscript_n->pos = *pos;

    return subscript_n;
}

subscript_np subscript_n_copy(const subscript_np node)
{
    subscript_np copy = malloc(sizeof(subscript_nt));

    copy->value = node_copy(&node->value);

    copy->pos = node_copy(&node->pos);

    return copy;
}

void subscript_n_free(subscript_np node)
{
    node_free(&node->pos);
    node_free(&node->value);
    free(node);
}

access_np access_n_set(node_p value, node_p property)
{
    access_np access_n = malloc(sizeof(access_nt));

    access_n->value = *value;
    access_n->property = *property;

    return access_n;
}

access_np access_n_copy(const access_np node)
{
    access_np copy = malloc(sizeof(access_nt));

    copy->value = node_copy(&node->value);

    copy->property = node_copy(&node->property);

    return copy;
}

void access_n_free(access_np node)
{
    node_free(&node->property);
    node_free(&node->value);
    free(node);
}

var_assign_np var_assign_n_set(char properties, char* name, unsigned char type, node_p value)
{
    var_assign_np var_assign_n = malloc(sizeof(var_assign_nt));

    var_assign_n->properties = properties;
    var_assign_n->name = name;
    var_assign_n->type = type;
    var_assign_n->value = *value;

    return var_assign_n;
}

var_assign_np var_assign_n_copy(const var_assign_np node)
{
    var_assign_np copy = malloc(sizeof(var_assign_nt));

    copy->properties = node->properties;

    copy->name = malloc(strlen(node->name) + 1);
    strcpy(copy->name, node->name);

    copy->type = node->type;
    copy->value = node_copy(&node->value);

    return copy;
}

void var_assign_n_free(var_assign_np node)
{
    node_free(&node->value);
    free(node);
}

var_fixed_assign_np var_fixed_assign_n_set(char properties, unsigned char operator, node_p var)
{
    var_fixed_assign_np var_fixed_assign_n = malloc(sizeof(var_fixed_assign_nt));

    var_fixed_assign_n->properties = properties;
    var_fixed_assign_n->operator = operator;
    var_fixed_assign_n->var = *var;

    return var_fixed_assign_n;
}

var_fixed_assign_np var_fixed_assign_n_copy(const var_fixed_assign_np node)
{
    var_fixed_assign_np copy = malloc(sizeof(var_fixed_assign_nt));

    copy->properties = node->properties;
    copy->operator = node->operator;

    copy->var = node_copy(&node->var);

    return copy;
}

void var_fixed_assign_n_free(var_fixed_assign_np node)
{
    node_free(&node->var);
    free(node);
}

var_reassign_np var_reassign_n_set(unsigned char operator, node_p var, node_p value)
{
    var_reassign_np var_reassign_n = malloc(sizeof(var_reassign_nt));

    var_reassign_n->operator = operator;
    var_reassign_n->var = *var;
    var_reassign_n->value = *value;

    return var_reassign_n;
}

var_reassign_np var_reassign_n_copy(const var_reassign_np node)
{
    var_reassign_np copy = malloc(sizeof(var_reassign_nt));

    copy->operator = node->operator;

    copy->var = node_copy(&node->var);
    copy->value = node_copy(&node->value);

    return copy;
}

void var_reassign_n_free(var_reassign_np node)
{
    node_free(&node->value);
    node_free(&node->var);
    free(node);
}

func_def_np func_def_n_set(char properties, char* name, arg_p args, unsigned long long size, unsigned char type, body_p body)
{
    func_def_np func_def_n = malloc(sizeof(func_def_nt));

    func_def_n->properties = properties;
    func_def_n->name = name;
    func_def_n->args = args;
    func_def_n->size = size;
    func_def_n->type = type;
    func_def_n->body = *body;

    return func_def_n;
}

func_def_np func_def_n_copy(const func_def_np node)
{
    func_def_np copy = malloc(sizeof(func_def_nt));

    copy->properties = node->properties;

    copy->name = malloc(strlen(node->name) + 1);
    strcpy(copy->name, node->name);

    copy->args = arg_p_copy(node->args, node->size);
    copy->size = node->size;

    copy->type = node->type;

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void func_def_n_free(func_def_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    arg_p_free(node->args, node->size);
    free(node);
}

func_call_np func_call_n_set(node_p func, arg_access_p args, unsigned long long size)
{
    func_call_np func_call_n = malloc(sizeof(func_call_nt));

    func_call_n->func = *func;
    func_call_n->args = args;
    func_call_n->size = size;

    return func_call_n;
}

func_call_np func_call_n_copy(const func_call_np node)
{
    func_call_np copy = malloc(sizeof(func_call_nt));

    copy->func = node_copy(&node->func);

    copy->args = arg_access_p_copy(node->args, node->size);
    copy->size = node->size;

    return copy;
}

void func_call_n_free(func_call_np node)
{
    arg_access_p_free(node->args, node->size);
    node_free(&node->func);
    free(node);
}

class_def_np class_def_n_set(char properties, char* name, body_p body)
{
    class_def_np class_def_n = malloc(sizeof(class_def_nt));

    class_def_n->properties = properties;
    class_def_n->name = name;
    class_def_n->body = *body;

    return class_def_n;
}

class_def_np class_def_n_copy(const class_def_np node)
{
    class_def_np copy = malloc(sizeof(class_def_nt));

    copy->properties = node->properties;

    copy->name = malloc(strlen(node->name) + 1);
    strcpy(copy->name, node->name);

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void class_def_n_free(class_def_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    free(node);
}

struct_def_np struct_def_n_set(char properties, char* name, body_p body)
{
    struct_def_np struct_def_n = malloc(sizeof(struct_def_nt));

    struct_def_n->properties = properties;
    struct_def_n->name = name;
    struct_def_n->body = *body;

    return struct_def_n;
}

struct_def_np struct_def_n_copy(const struct_def_np node)
{
    struct_def_np copy = malloc(sizeof(struct_def_nt));

    copy->properties = node->properties;

    copy->name = malloc(strlen(node->name) + 1);
    strcpy(copy->name, node->name);

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void struct_def_n_free(struct_def_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    free(node);
}

dollar_func_call_np dollar_func_call_n_set(char* name, node_p args, unsigned long long size)
{
    dollar_func_call_np dollar_func_call_n = malloc(sizeof(dollar_func_call_nt));

    dollar_func_call_n->name = name;
    dollar_func_call_n->args = args;
    dollar_func_call_n->size = size;

    return dollar_func_call_n;
}

dollar_func_call_np dollar_func_call_n_copy(const dollar_func_call_np node)
{
    dollar_func_call_np copy = malloc(sizeof(dollar_func_call_nt));

    copy->name = malloc(strlen(node->name) + 1);
    strcpy(copy->name, node->name);

    copy->args = node_p_copy(node->args, node->size);
    copy->size = node->size;

    return copy;
}

void dollar_func_call_n_free(dollar_func_call_np node)
{
    node_p_free1(node->args, node->size);
    free(node);
}

if_np if_n_set(case_p cases, unsigned long long size, body_p ebody)
{
    if_np if_n = malloc(sizeof(if_nt));

    if_n->cases = cases;
    if_n->size = size;
    if_n->ebody = *ebody;

    return if_n;
}

if_np if_n_copy(const if_np node)
{
    if_np copy = malloc(sizeof(if_nt));

    copy->cases = case_p_copy(node->cases, node->size);
    copy->size = node->size;

    copy->ebody.nodes = node_p_copy(node->ebody.nodes, node->ebody.size);
    copy->ebody.size = node->ebody.size;

    return copy;
}

void if_n_free(if_np node)
{
    node_p_free1(node->ebody.nodes, node->ebody.size);
    case_p_free(node->cases, node->size);
    free(node);
}

switch_np switch_n_set(node_p value, case_p cases, unsigned long long size, body_p dbody)
{
    switch_np switch_n = malloc(sizeof(switch_nt));

    switch_n->value = *value;
    switch_n->cases = cases;
    switch_n->size = size;
    switch_n->dbody = *dbody;

    return switch_n;
}

switch_np switch_n_copy(const switch_np node)
{
    switch_np copy = malloc(sizeof(switch_nt));

    copy->value = node_copy(&node->value);

    copy->cases = case_p_copy(node->cases, node->size);
    copy->size = node->size;

    copy->dbody.nodes = node_p_copy(node->dbody.nodes, node->dbody.size);
    copy->dbody.size = node->dbody.size;

    return copy;
}

void switch_n_free(switch_np node)
{
    node_p_free1(node->dbody.nodes, node->dbody.size);
    case_p_free(node->cases, node->size);
    node_free(&node->value);
    free(node);
}

for_np for_n_set(char* iterator, node_p start, node_p end, node_p step, body_p body)
{
    for_np for_n = malloc(sizeof(for_nt));

    for_n->iterator = iterator;
    for_n->start = *start;
    for_n->end = *end;
    for_n->step = *step;
    for_n->body = *body;

    return for_n;
}

for_np for_n_copy(const for_np node)
{
    for_np copy = malloc(sizeof(for_nt));

    copy->iterator = malloc(strlen(node->iterator) + 1);
    strcpy(copy->iterator, node->iterator);

    copy->start = node_copy(&node->start);
    copy->end = node_copy(&node->end);
    copy->step = node_copy(&node->step);

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void for_n_free(for_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    node_free(&node->step);
    node_free(&node->end);
    node_free(&node->start);
    free(node);
}

foreach_np foreach_n_set(char* iterator, node_p iterable, body_p body)
{
    foreach_np foreach_n = malloc(sizeof(foreach_nt));

    foreach_n->iterator = iterator;
    foreach_n->iterable = *iterable;
    foreach_n->body = *body;

    return foreach_n;
}

foreach_np foreach_n_copy(const foreach_np node)
{
    foreach_np copy = malloc(sizeof(foreach_nt));

    copy->iterator = malloc(strlen(node->iterator) + 1);
    strcpy(copy->iterator, node->iterator);

    copy->iterable = node_copy(&node->iterable);

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void foreach_n_free(foreach_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    node_free(&node->iterable);
    free(node);
}

loop_np loop_n_set(node_p init, node_p condition, node_p step, body_p body)
{
    loop_np loop_n = malloc(sizeof(loop_nt));

    loop_n->init = *init;
    loop_n->condition = *condition;
    loop_n->step = *step;
    loop_n->body = *body;

    return loop_n;
}

loop_np loop_n_copy(const loop_np node)
{
    loop_np copy = malloc(sizeof(loop_nt));

    copy->init = node_copy(&node->init);
    copy->condition = node_copy(&node->condition);
    copy->step = node_copy(&node->step);

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void loop_n_free(loop_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    node_free(&node->step);
    node_free(&node->condition);
    node_free(&node->init);
    free(node);
}

do_while_np do_while_n_set(body_p body, node_p condition)
{
    do_while_np do_while_n = malloc(sizeof(do_while_nt));

    do_while_n->body = *body;
    do_while_n->condition = *condition;

    return do_while_n;
}

do_while_np do_while_n_copy(const do_while_np node)
{
    do_while_np copy = malloc(sizeof(do_while_nt));

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    copy->condition = node_copy(&node->condition);

    return copy;
}

void do_while_n_free(do_while_np node)
{
    node_free(&node->condition);
    node_p_free1(node->body.nodes, node->body.size);
    free(node);
}

while_np while_n_set(node_p condition, body_p body)
{
    while_np while_n = malloc(sizeof(while_nt));

    while_n->condition = *condition;
    while_n->body = *body;

    return while_n;
}

while_np while_n_copy(const while_np node)
{
    while_np copy = malloc(sizeof(while_nt));

    copy->condition = node_copy(&node->condition);

    copy->body.nodes = node_p_copy(node->body.nodes, node->body.size);
    copy->body.size = node->body.size;

    return copy;
}

void while_n_free(while_np node)
{
    node_p_free1(node->body.nodes, node->body.size);
    node_free(&node->condition);
    free(node);
}

try_np try_n_set(body_p tbody, case_p excepts, unsigned long long size, body_p fbody)
{
    try_np try_n = malloc(sizeof(try_nt));

    try_n->tbody = *tbody;
    try_n->excepts = excepts;
    try_n->size = size;
    try_n->fbody = *fbody;

    return try_n;
}

try_np try_n_copy(const try_np node)
{
    try_np copy = malloc(sizeof(try_nt));

    copy->tbody.nodes = node_p_copy(node->tbody.nodes, node->tbody.size);
    copy->tbody.size = node->tbody.size;

    copy->excepts = case_p_copy(node->excepts, node->size);
    copy->size = node->size;

    copy->fbody.nodes = node_p_copy(node->fbody.nodes, node->fbody.size);
    copy->fbody.size = node->fbody.size;

    return copy;
}

void try_n_free(try_np node)
{
    node_p_free1(node->fbody.nodes, node->fbody.size);
    case_p_free(node->excepts, node->size);
    node_p_free1(node->tbody.nodes, node->tbody.size);
    free(node);
}

return_np return_n_set(node_p value)
{
    return_np return_n = malloc(sizeof(return_nt));

    return_n->value = *value;

    return return_n;
}

return_np return_n_copy(const return_np node)
{
    return_np copy = malloc(sizeof(return_nt));

    copy->value = node_copy(&node->value);

    return copy;
}

void return_n_free(return_np node)
{
    if (!node)
        return;

    node_free(&node->value);
    free(node);
}

void body_print(FILE* stream, body_p body)
{
    fputc('{', stream);
    node_p_print(stream, body->nodes, body->size);
    fputc('}', stream);
}

void node_p_print(FILE* stream, node_p nodes, unsigned long long size)
{
    if (!size)
        return;

    node_print(stream, nodes);

    unsigned long long i;
    for (i = 1; i < size; i++)
    {
        fputs(", ", stream);
        node_print(stream, nodes + i);
    }
}

void pair_p_print(FILE* stream, pair_p pairs, unsigned long long size)
{
    node_print(stream, &pairs->key);
    fputs(": ", stream);
    node_print(stream, &pairs->value);

    unsigned long long i;
    for (i = 1; i < size; i++)
    {
        fputs(", ", stream);
        node_print(stream, &pairs[i].key);
        fputs(": ", stream);
        node_print(stream, &pairs[i].value);
    }
}

void arg_p_print(FILE* stream, arg_p args, unsigned long long size)
{
    if (!size)
        return;

    fprintf(stream, "(%s, %s, ", args->name, token_labels[args->type]);
    node_print(stream, &args->value);
    putc(')', stream);

    unsigned long long i;
    for (i = 1; i < size; i++)
    {
        fprintf(stream, ", (%s, %s, ", args[i].name, token_labels[args[i].type]);
        node_print(stream, &args[i].value);
        putc(')', stream);
    }
}

void arg_access_p_print(FILE* stream, arg_access_p args, unsigned long long size)
{
    if (!size)
        return;

    fprintf(stream, "(%s, ", args->name);
    node_print(stream, &args->value);
    putc(')', stream);

    unsigned long long i;
    for (i = 1; i < size; i++)
    {
        fprintf(stream, ", (%s, ", args[i].name);
        node_print(stream, &args[i].value);
        putc(')', stream);
    }
}

void case_p_print(FILE* stream, case_p cases, unsigned long long size)
{
    if (!size)
        return;

    fputc('(', stream);
    node_print(stream, &cases->condition);
    fputs(", ", stream);
    body_print(stream, &cases->body);
    fputc(')', stream);

    unsigned long long i;
    for (i = 1; i < size; i++)
    {
        fputs(", (", stream);
        node_print(stream, &cases[i].condition);
        fputs(", ", stream);
        body_print(stream, &cases[i].body);
        fputc(')', stream);
    }
}
