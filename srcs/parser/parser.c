/*/
 * MetaReal version 1.0.0
/*/

#include <parser/parser.h>
#include <stdlib.h>
#include <def.h>

#define advance_newline(t) if (t->type == NEWLINE_T) t++

void pres_fail(pres_p pres, invalid_syntax_p error);

token_p dollar_func(pres_p pres, token_p tokens); // dollar-function
token_p statement(pres_p res, token_p tokens);    // return continue break
token_p tuple(pres_p res, token_p tokens);        // tuple
token_p assign(pres_p res, token_p tokens);       // = += -= *= /= %= //= **= &= |= ^= <<= >>=
token_p ternary(pres_p res, token_p tokens);      // ?:
token_p typeof(pres_p pres, token_p tokens);      // is are
token_p contain(pres_p pres, token_p tokens);     // in
token_p or(pres_p pres, token_p tokens);          // || or
token_p xor(pres_p pres, token_p tokens);         // ^^ xor
token_p and(pres_p pres, token_p tokens);         // && and
token_p compare1(pres_p pres, token_p tokens);    // == !=
token_p compare2(pres_p pres, token_p tokens);    // < > <= >=
token_p b_or(pres_p pres, token_p tokens);        // |
token_p b_xor(pres_p pres, token_p tokens);       // ^
token_p b_and(pres_p pres, token_p tokens);       // &
token_p shift(pres_p pres, token_p tokens);       // << >>
token_p expression(pres_p pres, token_p tokens);  // + -
token_p term(pres_p pres, token_p tokens);        // * / % //
token_p factor(pres_p pres, token_p tokens);      // +(unary) -(unary) ~ ! not ++ --
token_p exponent(pres_p pres, token_p tokens);    // **
token_p post(pres_p pres, token_p tokens);        // array-subscripting ++(post) --(post) function-call access
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
    } while ((tokens - 1)->type == NEWLINE_T || tokens->type == SEMICOLON_T);

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
    return statement(pres, tokens);
}

token_p statement(pres_p pres, token_p tokens)
{
    return tuple(pres, tokens);
}

token_p tuple(pres_p pres, token_p tokens)
{
    return assign(pres, tokens);
}

token_p assign(pres_p pres, token_p tokens)
{
    return ternary(pres, tokens);
}

token_p ternary(pres_p pres, token_p tokens)
{
    tokens = typeof(pres, tokens);
    if (pres->has_error)
        return tokens;

    if (tokens->type == QUESTION_T)
    {
        tokens++;
        advance_newline(tokens);

        node_t condition = *pres->nodes;

        node_t left;
        node_t right;

        if (tokens->type == COLON_T)
        {
            tokens++;
            advance_newline(tokens);

            tokens = dollar_func(pres, tokens);
            if (pres->has_error)
                return tokens;

            left.type = NULL_N;
            right = *pres->nodes;
        }
        else
        {
            tokens = dollar_func(pres, tokens);
            if (pres->has_error)
                return tokens;

            left = *pres->nodes;

            if (tokens->type == COLON_T)
            {
                tokens++;
                advance_newline(tokens);

                tokens = dollar_func(pres, tokens);
                if (pres->has_error)
                    return tokens;

                right = *pres->nodes;
            }
            else
                right.type = NULL_N;
        }

        ternary_condition_np node = ternary_condition_n_set(&condition, &left, &right);
        *pres->nodes = node_set1(TERNARY_CONDITION_N, node, &condition.poss, &pres->nodes->pose);
        return tokens;
    }

    return tokens;
}

token_p typeof(pres_p pres, token_p tokens)
{
    tokens = contain(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == IS_TK || tokens->type == ARE_TK)
    {
        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = contain(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(operator, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p contain(pres_p pres, token_p tokens)
{
    tokens = or(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == IN_TK)
    {
        tokens++;
        advance_newline(tokens);

        tokens = or(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(IN_TK, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p or(pres_p pres, token_p tokens)
{
    tokens = xor(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == OR_T || tokens->type == OR_TK)
    {
        tokens++;
        advance_newline(tokens);

        tokens = xor(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(OR_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p xor(pres_p pres, token_p tokens)
{
    tokens = and(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == XOR_T || tokens->type == XOR_TK)
    {
        tokens++;
        advance_newline(tokens);

        tokens = and(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(XOR_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p and(pres_p pres, token_p tokens)
{
    tokens = compare1(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == AND_T || tokens->type == AND_TK)
    {
        tokens++;
        advance_newline(tokens);

        tokens = compare1(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(AND_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p compare1(pres_p pres, token_p tokens)
{
    tokens = compare2(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == EQUAL_T || tokens->type == NEQUAL_T)
    {
        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = compare2(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(operator, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p compare2(pres_p pres, token_p tokens)
{
    tokens = b_or(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type >= LESS_T && tokens->type <= GREATER_EQ_T)
    {
        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = b_or(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(operator, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p b_or(pres_p pres, token_p tokens)
{
    tokens = b_xor(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == B_OR_T)
    {
        tokens++;
        advance_newline(tokens);

        tokens = b_xor(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(B_OR_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p b_xor(pres_p pres, token_p tokens)
{
    tokens = b_and(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == B_XOR_T)
    {
        tokens++;
        advance_newline(tokens);

        tokens = b_and(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(B_XOR_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p b_and(pres_p pres, token_p tokens)
{
    tokens = shift(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == B_AND_T)
    {
        tokens++;
        advance_newline(tokens);

        tokens = shift(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(B_AND_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p shift(pres_p pres, token_p tokens)
{
    tokens = expression(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == LSHIFT_T || tokens->type == RSHIFT_T)
    {
        unsigned char operator = tokens++->type;

        advance_newline(tokens);

        tokens = expression(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(operator, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p expression(pres_p pres, token_p tokens)
{
    tokens = term(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

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

    return exponent(pres, tokens);
}

token_p exponent(pres_p pres, token_p tokens)
{
    tokens = post(pres, tokens);
    if (pres->has_error)
        return tokens;

    node_t left = *pres->nodes;

    while (tokens->type == POWER_T)
    {
        tokens++;
        advance_newline(tokens);

        tokens = factor(pres, tokens);
        if (pres->has_error)
            return tokens;

        left.value = binary_operation_n_set(POWER_T, &left, pres->nodes);
        left.type = BINARY_OPERATION_N;
        left.pose = pres->nodes->pose;
    }

    *pres->nodes = left;
    return tokens;
}

token_p post(pres_p pres, token_p tokens)
{
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

        advance_newline(tokens);

        return tokens;
    }

    if (tokens->type == INT_T)
    {
        int_np node = int_n_set(tokens->value, tokens->size);
        *pres->nodes = node_set1(INT_N, node, &tokens->poss, &tokens->pose);

        tokens++;
        advance_newline(tokens);

        return tokens;
    }
    if (tokens->type == FLOAT_N)
    {
        int_np node = int_n_set(tokens->value, tokens->size);
        *pres->nodes = node_set1(INT_N, node, &tokens->poss, &tokens->pose);

        tokens++;
        advance_newline(tokens);

        return tokens;
    }
    if (tokens->type == COMPLEX_N)
    {
        int_np node = int_n_set(tokens->value, tokens->size);
        *pres->nodes = node_set1(INT_N, node, &tokens->poss, &tokens->pose);

        tokens++;
        advance_newline(tokens);

        return tokens;
    }

    invalid_syntax_t error = invalid_syntax_set(NULL, &tokens->poss, &tokens->pose);
    pres_fail(pres, &error);
    return tokens;
}
