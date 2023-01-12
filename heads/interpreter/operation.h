/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_OPERATION__
#define __M_OPERATION__

#include "interpreter.h"

ires_t operate_add(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_subtract(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose);
ires_t operate_multiply(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_divide(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose);
ires_t operate_modulo(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose);
ires_t operate_quotient(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p rposs, pos_p rpose);
ires_t operate_power(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);

ires_t operate_b_and(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_b_or(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_b_xor(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_lshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_rshift(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);

ires_t operate_equal(value_p left, value_p right);
ires_t operate_nequal(value_p left, value_p right);
ires_t operate_less(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_greater(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_less_eq(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_greater_eq(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);

ires_t operate_xor(value_p left, value_p right);

ires_t operate_contain(value_p left, value_p right, pos_p poss, pos_p pose, context_p context);
ires_t operate_is(value_p left, value_p right);
ires_t operate_are(value_p left, value_p right, pos_p poss, pos_p pose, context_p context, pos_p lposs, pos_p lpose);

ires_t operate_subscript(value_p left, value_p right, context_p context, pos_p lposs, pos_p lpose, pos_p rposs, pos_p rpose);
ires_t operate_subscript_ptr(value_p left, value_p right, context_p context, pos_p lposs, pos_p lpose, pos_p rposs, pos_p rpose);



ires_t operate_positive(value_p operand);
ires_t operate_negate(value_p operand, pos_p poss, pos_p pose, context_p context);

ires_t operate_b_not(value_p operand, pos_p poss, pos_p pose, context_p context);

ires_t operate_not(value_p operand);

ires_t operate_increment(value_p operand, pos_p poss, pos_p pose, context_p context);
ires_t operate_decrement(value_p operand, pos_p poss, pos_p pose, context_p context);



void operate_success(value_p left, const value_p right);
char operate_success_type_change(const value_p left, const value_p right);

char operate_equal_compare(const value_p left, const value_p right);
char operate_less_compare(const value_p left, const value_p right);
char operate_greater_compare(const value_p left, const value_p right);



char operate_sign(const value_p operand);

unsigned long long operate_size(const value_p operand);
value_t operate_index(const value_p operand, unsigned long long index);

#endif /* __M_OPERATION__ */
