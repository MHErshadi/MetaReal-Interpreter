/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_OPERATION__
#define __M_OPERATION__

#include "interpreter.h"

ires_t operate_add(value_p left, value_p right);
ires_t operate_subtract(value_p left, value_p right);
ires_t operate_multiply(value_p left, value_p right);
ires_t operate_divide(value_p left, value_p right);
ires_t operate_modulo(value_p left, value_p right);
ires_t operate_quotient(value_p left, value_p right);
ires_t operate_power(value_p left, value_p right);

ires_t operate_b_and(value_p left, value_p right);
ires_t operate_b_or(value_p left, value_p right);
ires_t operate_b_xor(value_p left, value_p right);
ires_t operate_b_lshift(value_p left, value_p right);
ires_t operate_b_rshift(value_p left, value_p right);

ires_t operate_equal(value_p left, value_p right);
ires_t operate_nequal(value_p left, value_p right);
ires_t operate_less(value_p left, value_p right);
ires_t operate_greater(value_p left, value_p right);
ires_t operate_less_eq(value_p left, value_p right);
ires_t operate_greater_eq(value_p left, value_p right);

ires_t operate_and(value_p left, value_p right);
ires_t operate_or(value_p left, value_p right);
ires_t operate_xor(value_p left, value_p right);

/* */

ires_t operate_negate(value_p operand);

ires_t operate_b_not(value_p operand);

ires_t operate_not(value_p operand);

ires_t operate_increment(value_p operand);
ires_t operate_decrement(value_p operand);

#endif /* __M_OPERATION__ */
