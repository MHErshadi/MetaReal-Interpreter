/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_RUNTIME_ERROR__
#define __M_RUNTIME_ERROR__

#include "errlib.h"

runtime_t invalid_access_object(unsigned char type,
    pos_p poss, pos_p pose, context_p context);
runtime_t invalid_access_statement(const char* statement,
    pos_p poss, pos_p pose, context_p context);

runtime_t type_match(unsigned char type1, unsigned char type2,
    pos_p poss, pos_p pose, context_p context);

runtime_t const_variable(const char* name,
    pos_p poss, pos_p pose, context_p context);
runtime_t type_specified_variable(const char* name, unsigned char type,
    pos_p poss, pos_p pose, context_p context);
runtime_t assign_type_specified_variable(unsigned char type1, unsigned char type2,
    pos_p poss, pos_p pose, context_p context);

runtime_t not_defined(const char* name,
    pos_p poss, pos_p pose, context_p context);

runtime_t invalid_type(const char* name, const char* types, unsigned char type,
    pos_p poss, pos_p pose, context_p context);

runtime_t out_of_boundary(const char* name, unsigned long long lbound, unsigned long long rbound,
    pos_p poss, pos_p pose, context_p context);

runtime_t outside_func(pos_p poss, pos_p pose, context_p context);
runtime_t outside_loop(const char* name,
    pos_p poss, pos_p pose, context_p context);

runtime_t invalid_arg_number_function(const char* name, unsigned long long min, unsigned long long max, unsigned long long size,
    pos_p poss, pos_p pose, context_p context);

runtime_t invalid_dollar_function(const char* name,
    pos_p poss, pos_p pose, context_p context);
runtime_t invalid_arg_number(const char* name, unsigned long long size1, unsigned long long size2,
    pos_p poss, pos_p pose, context_p context);
runtime_t invalid_param_name(const char* name,
    pos_p poss, pos_p pose, context_p context);

runtime_t illegal_operation(unsigned char type1, unsigned char type2, const char* operator,
    pos_p poss, pos_p pose, context_p context);
runtime_t illegal_operation_unary(unsigned char type, const char* operator,
    pos_p poss, pos_p pose, context_p context);
runtime_t mem_overflow(pos_p poss, pos_p pose, context_p context);
runtime_t out_of_range(pos_p poss, pos_p pose, context_p context);
runtime_t division_by_zero(pos_p poss, pos_p pose, context_p context);
runtime_t modulo_by_zero(pos_p poss, pos_p pose, context_p context);

#endif /* __M_RUNTIME_ERROR__ */
