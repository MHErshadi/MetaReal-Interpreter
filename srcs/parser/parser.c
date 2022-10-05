/*/
 * MetaReal version 1.0.0
/*/

#include <parser/parser.h>
#include <stdlib.h>
#include <def.h>

#define advance_newline(t) if (t->type == NEWLINE_T) t++

void pres_fail(pres_p pres, invalid_syntax_p error);

token_p dollar_func(pres_p pres, token_p tokens); // dollar-function
token_p expression(pres_p pres, token_p tokens);  // + -
token_p term(pres_p pres, token_p tokens);        // * / % //
token_p factor(pres_p pres, token_p tokens);      // +(un) -(un) ~ ! not ++ --
token_p core(pres_p pres, token_p tokens);        // statements types

pres_t parse(token_p tokens)
{
    pres_t pres;
    pres.has_error = 0;

    token_p tokens_copy = tokens;

    pres.nodes = malloc(NODES_SIZE * sizeof(node_t));

    unsigned long long alloc = NODES_SIZE;
    unsigned long long size = 0;
    do
    {
        for (; tokens->type == NEWLINE_T || tokens->type == SEMICOLON_T; tokens++);
        if (tokens->type == EOF_T)
            break;

        if (size == alloc)
            pres.nodes = realloc(pres.nodes, (alloc += NODES_SIZE) * sizeof(node_t));

        tokens = dollar_func(&pres, tokens);
        if (pres.has_error)
        {
            pres.nodes -= size;
            free(pres.nodes);
            free(tokens_copy);
            return pres;
        }

        size++;
        pres.nodes++;
    } while (tokens->type == NEWLINE_T || tokens->type == SEMICOLON_T);

    if (tokens->type != EOF_T)
    {
        pres.nodes -= size;
        free(pres.nodes);
        free(tokens_copy);

        invalid_syntax_t error = invalid_syntax_set("Expected EOF", &tokens->poss, &tokens->pose);
        pres_fail(&pres, &error);
        return pres;
    }

    pres.nodes -= size;
    if (size + 1 != alloc)
        pres.nodes = realloc(pres.nodes, (size + 1) * sizeof(node_t));

    pres.nodes[size] = node_set2(NULL_N, &tokens->poss, &tokens->pose);

    free(tokens_copy);
    return pres;
}

void pres_fail(pres_p pres, invalid_syntax_p error)
{
    pres->error = *error;
    pres->has_error = 1;
}

token_p dollar_func(pres_p pres, token_p tokens)
{
    return expression(pres, tokens);
}

token_p expression(pres_p pres, token_p tokens)
{
    tokens = term(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    advance_newline(tokens);
    while (tokens->type == PLUS_T || tokens->type == MINUS_T)
    {
        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = term(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(operator, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p term(pres_p pres, token_p tokens)
{
    tokens = factor(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    advance_newline(tokens);
    while (tokens->type >= MULTIPLY_T && tokens->type <= QUOTIENT_T)
    {
        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = factor(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(operator, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p factor(pres_p pres, token_p tokens)
{
    if (tokens->type == PLUS_T || tokens->type == MINUS_T ||
        tokens->type == B_NOT_T ||
        tokens->type == NOT_T || tokens->type == NOT_TK)
    {
        pos_p poss = &tokens->poss;

        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = factor(pres, tokens);
        if (pres->has_error)
            return tokens;

        unary_operation_np node = unary_operation_n_set(operator, pres->nodes);
        *pres->nodes = node_set1(UNARY_OPERATION_N, node, poss, &pres->nodes->pose);
        return tokens;
    }

    if (tokens->type == INCREMENT_T || tokens->type == DECREMENT_T)
    {
        pos_p poss = &tokens->poss;

        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = factor(pres, tokens);
        if (pres->has_error)
            return tokens;

        var_fixed_assign_np node = var_fixed_assign_n_set(VFA_PROP(0), operator, pres->nodes);
        *pres->nodes = node_set1(VAR_FIXED_ASSIGN_N, node, poss, &pres->nodes->pose);
        return tokens;
    }

    return core(pres, tokens);
}

token_p core(pres_p pres, token_p tokens)
{
    if (tokens->type == LPAREN_T)
    {
        pos_p poss = &tokens++->poss;

        advance_newline(tokens);

        tokens = dollar_func(pres, tokens);
        if (pres->has_error)
            return tokens;

        if (tokens->type != RPAREN_T)
        {
            invalid_syntax_t error = invalid_syntax_set("Expected ')'", &tokens->poss, &tokens->pose);
            pres_fail(pres, &error);
            return tokens;
        }

        pres->nodes->poss = *poss;
        pres->nodes->pose = tokens++->pose;

        return tokens;
    }

    if (tokens->type == INT_T)
    {
        int_np node = int_n_set(tokens->value, tokens->size);
        *pres->nodes = node_set1(INT_N, node, &tokens->poss, &tokens->pose);

        return ++tokens;
    }
    if (tokens->type == FLOAT_N)
    {
        int_np node = int_n_set(tokens->value, tokens->size);
        *pres->nodes = node_set1(INT_N, node, &tokens->poss, &tokens->pose);

        return ++tokens;
    }
    if (tokens->type == COMPLEX_N)
    {
        int_np node = int_n_set(tokens->value, tokens->size);
        *pres->nodes = node_set1(INT_N, node, &tokens->poss, &tokens->pose);

        return ++tokens;
    }

    invalid_syntax_t error = invalid_syntax_set(NULL, &tokens->poss, &tokens->pose);
    pres_fail(pres, &error);
    return tokens;
}
