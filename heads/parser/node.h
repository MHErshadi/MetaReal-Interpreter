/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_NODE__
#define __M_NODE__

#include <debugger/pos.h>
#include <stdio.h>

#define PROP_PUBLIC(x) (x & 1)
#define PROP_GLOBAL(x) (x >> 1 & 1)
#define PROP_CONST(x)  (x >> 2 & 1)
#define PROP_STATIC(x) (x >> 3 & 1)

#define VFA_PROP(post) 0b ## post

#define PROP_POST(x) (x & 1)

enum _node_types_
{
    NULL_N,

    /* */

    NONE_N,

    INT_N,
    FLOAT_N,
    COMPLEX_N,

    BOOL_N,

    CHAR_N,

    STR_N,

    LIST_N,
    TUPLE_N,
    DICT_N,
    SET_N,

    TYPE_N,

    /* */

    BINARY_OPERATION_N,
    UNARY_OPERATION_N,

    TERNARY_CONDITION_N,

    SUBSCRIPT_N,

    ACCESS_N,

    /* */

    VAR_ASSIGN_N,
    VAR_FIXED_ASSIGN_N,
    VAR_REASSIGN_N,
    VAR_ACCESS_N,

    FUNC_DEF_N,
    FUNC_CALL_N,

    CLASS_DEF_N,

    STRUCT_DEF_N,

    DOLLAR_FUNC_CALL_N,

    /* */

    IF_N,

    SWITCH_N,

    FOR_N,
    FOREACH_N,
    LOOP_N,

    DO_WHILE_N,
    WHILE_N,

    TRY_N,

    IMPORT_N,
    INCLUDE_N,

    /* */

    RETURN_N,

    CONTINUE_N,
    BREAK_N
};

union __union_value__
{
    void* ptr;
    unsigned char chr;
};
typedef union __union_value__ union_value_t;

struct __node__
{
    unsigned char type;
    union_value_t value;

    pos_t poss;
    pos_t pose;
};
typedef struct __node__ node_t;
typedef struct __node__* node_p;

struct __pair__
{
    node_t key;
    node_t value;
};
typedef struct __pair__ pair_t;
typedef struct __pair__* pair_p;

struct __arg__
{
    char* name;

    unsigned char type;
    node_t value;
};
typedef struct __arg__ arg_t;
typedef struct __arg__* arg_p;

struct __body__
{
    node_p nodes;
    unsigned long long size;
};
typedef struct __body__ body_t;
typedef struct __body__* body_p;

struct __arg_access__
{
    char* name;

    node_t value;
};
typedef struct __arg_access__ arg_access_t;
typedef struct __arg_access__* arg_access_p;

struct __case__
{
    node_t condition;

    body_t body;
};
typedef struct __case__ case_t;
typedef struct __case__* case_p;

/* */

struct __int_n__
{
    char* value;
    unsigned long long size;
};
typedef struct __int_n__ int_nt;
typedef struct __int_n__* int_np;

struct __float_n__
{
    char* value;
    unsigned long long size;
};
typedef struct __float_n__ float_nt;
typedef struct __float_n__* float_np;

struct __complex_n__
{
    char* value;
    unsigned long long size;
};
typedef struct __complex_n__ complex_nt;
typedef struct __complex_n__* complex_np;

struct __str_n__
{
    char* value;
    unsigned long long size;
};
typedef struct __str_n__ str_nt;
typedef struct __str_n__* str_np;

struct __list_n__
{
    node_p elements;
    unsigned long long size;
};
typedef struct __list_n__ list_nt;
typedef struct __list_n__* list_np;

struct __tuple_n__
{
    node_p elements;
    unsigned long long size;
};
typedef struct __tuple_n__ tuple_nt;
typedef struct __tuple_n__* tuple_np;

struct __dict_n__
{
    pair_p elements;
    unsigned long long size;
};
typedef struct __dict_n__ dict_nt;
typedef struct __dict_n__* dict_np;

struct __set_n__
{
    node_p elements;
    unsigned long long size;
};
typedef struct __set_n__ set_nt;
typedef struct __set_n__* set_np;

struct __binary_operation_n__
{
    unsigned char operator;

    node_t left;
    node_t right;
};
typedef struct __binary_operation_n__ binary_operation_nt;
typedef struct __binary_operation_n__* binary_operation_np;

struct __unary_operation_n__
{
    unsigned char operator;

    node_t operand;
};
typedef struct __unary_operation_n__ unary_operation_nt;
typedef struct __unary_operation_n__* unary_operation_np;

struct __ternary_condition_n__
{
    node_t condition;

    node_t left;
    node_t right;
};
typedef struct __ternary_condition_n__ ternary_condition_nt;
typedef struct __ternary_condition_n__* ternary_condition_np;

struct __subscript_n__
{
    node_t value;

    node_t pos;
};
typedef struct __subscript_n__ subscript_nt;
typedef struct __subscript_n__* subscript_np;

struct __access_n__
{
    node_t value;

    node_t property;
};
typedef struct __access_n__ access_nt;
typedef struct __access_n__* access_np;

struct __var_assign_n__
{
    char properties;
    char* name;

    unsigned char type;
    node_t value;
};
typedef struct __var_assign_n__ var_assign_nt;
typedef struct __var_assign_n__* var_assign_np;

struct __var_fixed_assign_n__
{
    char properties;
    unsigned char operator;

    node_t var;
};
typedef struct __var_fixed_assign_n__ var_fixed_assign_nt;
typedef struct __var_fixed_assign_n__* var_fixed_assign_np;

struct __var_reassign_n__
{
    unsigned char operator;

    node_t var;
    node_t value;
};
typedef struct __var_reassign_n__ var_reassign_nt;
typedef struct __var_reassign_n__* var_reassign_np;

struct __func_def_n__
{
    char properties;
    char* name;

    arg_p args;
    unsigned long long size;

    unsigned char type;
    body_t body;
};
typedef struct __func_def_n__ func_def_nt;
typedef struct __func_def_n__* func_def_np;

struct __func_call_n__
{
    node_t func;

    arg_access_p args;
    unsigned long long size;
};
typedef struct __func_call_n__ func_call_nt;
typedef struct __func_call_n__* func_call_np;

struct __class_def_n__
{
    char properties;
    char* name;

    body_t body;
};
typedef struct __class_def_n__ class_def_nt;
typedef struct __class_def_n__* class_def_np;

struct __struct_def_n__
{
    char properties;
    char* name;

    body_t body;
};
typedef struct __struct_def_n__ struct_def_nt;
typedef struct __struct_def_n__* struct_def_np;

struct __dollar_func_call_n__
{
    char* name;

    node_p args;
    unsigned long long size;
};
typedef struct __dollar_func_call_n__ dollar_func_call_nt;
typedef struct __dollar_func_call_n__* dollar_func_call_np;

struct __if_n__
{
    case_p cases;
    unsigned long long size;

    body_t ebody;
};
typedef struct __if_n__ if_nt;
typedef struct __if_n__* if_np;

struct __switch_n__
{
    node_t value;

    case_p cases;
    unsigned long long size;

    body_t dbody;
};
typedef struct __switch_n__ switch_nt;
typedef struct __switch_n__* switch_np;

struct __for_n__
{
    char* iterator;

    node_t start;
    node_t end;
    node_t step;

    body_t body;
};
typedef struct __for_n__ for_nt;
typedef struct __for_n__* for_np;

struct __foreach_n__
{
    char* iterator;

    node_t iterable;

    body_t body;
};
typedef struct __foreach_n__ foreach_nt;
typedef struct __foreach_n__* foreach_np;

struct __loop_n__
{
    node_t init;
    node_t condition;
    node_t step;

    body_t body;
};
typedef struct __loop_n__ loop_nt;
typedef struct __loop_n__* loop_np;

struct __do_while_n__
{
    body_t body;

    node_t condition;
};
typedef struct __do_while_n__ do_while_nt;
typedef struct __do_while_n__* do_while_np;

struct __while_n__
{
    node_t condition;

    body_t body;
};
typedef struct __while_n__ while_nt;
typedef struct __while_n__* while_np;

struct __try_n__
{
    body_t tbody;

    case_p excepts;
    unsigned long long size;

    body_t fbody;
};
typedef struct __try_n__ try_nt;
typedef struct __try_n__* try_np;

struct __return_n__
{
    node_t value;
};
typedef struct __return_n__ return_nt;
typedef struct __return_n__* return_np;

node_t node_set1(unsigned char type, void* value, pos_p poss, pos_p pose);
node_t node_set2(unsigned char type, unsigned char value, pos_p poss, pos_p pose);
node_t node_set3(unsigned char type, pos_p poss, pos_p pose);

void node_free(node_p node);

void node_print(FILE* stream, node_p node);

void node_p_free1(node_p nodes, unsigned long long size);
void node_p_free2(node_p nodes);

body_t body_set(node_p node);

void pair_p_free(pair_p pairs, unsigned long long size);
void arg_p_free(arg_p args, unsigned long long size);
void arg_access_p_free(arg_access_p args, unsigned long long size);
void case_p_free(case_p cases, unsigned long long size);

/* */

int_np int_n_set(char* value, unsigned long long size);
void int_n_free(int_np node);

float_np float_n_set(char* value, unsigned long long size);
void float_n_free(float_np node);

complex_np complex_n_set(char* value, unsigned long long size);
void complex_n_free(complex_np node);

str_np str_n_set(char* value, unsigned long long size);
void str_n_free(str_np node);

list_np list_n_set(node_p elements, unsigned long long size);
void list_n_free(list_np node);

tuple_np tuple_n_set(node_p elements, unsigned long long size);
void tuple_n_free(tuple_np node);

dict_np dict_n_set(pair_p elements, unsigned long long size);
void dict_n_free(dict_np node);

set_np set_n_set(node_p elements, unsigned long long size);
void set_n_free(set_np node);

binary_operation_np binary_operation_n_set(unsigned char operator, node_p left, node_p right);
void binary_operation_n_free(binary_operation_np node);

unary_operation_np unary_operation_n_set(unsigned char operator, node_p operand);
void unary_operation_n_free(unary_operation_np node);

ternary_condition_np ternary_condition_n_set(node_p condition, node_p left, node_p right);
void ternary_condition_n_free(ternary_condition_np node);

subscript_np subscript_n_set(node_p value, node_p pos);
void subscript_n_free(subscript_np node);

access_np access_n_set(node_p value, node_p property);
void access_n_free(access_np node);

var_assign_np var_assign_n_set(char properties, char* name, unsigned char type, node_p value);
void var_assign_n_free(var_assign_np node);

var_fixed_assign_np var_fixed_assign_n_set(char properties, unsigned char operator, node_p var);
void var_fixed_assign_n_free(var_fixed_assign_np node);

var_reassign_np var_reassign_n_set(unsigned char operator, node_p var, node_p value);
void var_reassign_n_free(var_reassign_np node);

func_def_np func_def_n_set(char properties, char* name, arg_p args, unsigned long long size, unsigned char type, body_p body);
void func_def_n_free(func_def_np node);

func_call_np func_call_n_set(node_p func, arg_access_p args, unsigned long long size);
void func_call_n_free(func_call_np node);

class_def_np class_def_n_set(char properties, char* name, body_p body);
void class_def_n_free(class_def_np node);

struct_def_np struct_def_n_set(char properties, char* name, body_p body);
void struct_def_n_free(struct_def_np node);

dollar_func_call_np dollar_func_call_n_set(char* name, node_p args, unsigned long long size);
void dollar_func_call_n_free(dollar_func_call_np node);

if_np if_n_set(case_p cases, unsigned long long size, body_p ebody);
void if_n_free(if_np node);

switch_np switch_n_set(node_p value, case_p cases, unsigned long long size, body_p dbody);
void switch_n_free(switch_np node);

for_np for_n_set(char* iterator, node_p start, node_p end, node_p step, body_p body);
void for_n_free(for_np node);

foreach_np foreach_n_set(char* iterator, node_p iterable, body_p body);
void foreach_n_free(foreach_np node);

loop_np loop_n_set(node_p init, node_p condition, node_p step, body_p body);
void loop_n_free(loop_np node);

do_while_np do_while_n_set(body_p body, node_p condition);
void do_while_n_free(do_while_np node);

while_np while_n_set(node_p condition, body_p body);
void while_n_free(while_np node);

try_np try_n_set(body_p tbody, case_p excepts, unsigned long long size, body_p fbody);
void try_n_free(try_np node);

return_np return_n_set(node_p value);
void return_n_free(return_np node);

#endif /* __M_NODE__ */
