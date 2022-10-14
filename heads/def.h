/*/
 * MetaReal version 1.0.0
/*/

#ifndef __M_DEF__
#define __M_DEF__

#define CMD_FILE_NAME "<stdin>"
#define CMD_INPUT_SIZE 1025

#define STACK_SIZE 524288 // 512 * 1024
#define HEAP_SIZE 1048576 // 1024 * 1024

/* */

#define TOKENS_SIZE 8192

#define IDENTIFIER_SIZE 16
#define NUMBER_SIZE 8
#define STRING_SIZE 32

/* */

#define NODES_SIZE 2048

#define LIST_SIZE 8
#define TUPLE_SIZE 8
#define DICT_SIZE 8
#define SET_SIZE 8

#define FUNC_DEF_SIZE 4
#define FUNC_DEF_BODY_SIZE 16

#define CLASS_DEF_BODY_SIZE 32

#define STRUCT_DEF_BODY_SIZE 8

#define FUNC_CALL_SIZE 4

#define DOLLAR_FUNC_ARGS_SIZE 4

#endif /* __M_DEF__ */
