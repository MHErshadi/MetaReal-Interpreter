/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
/*/

#ifndef __M_MEMORY__
#define __M_MEMORY__

/* stack */

struct __stack__
{
    char* data;
    unsigned long long size;

    char* sp;

    struct __stack__* temp;
};
typedef struct __stack__ stack_t;
typedef struct __stack__* stack_p;

void stack_init(stack_p stack, unsigned long long size);
void stack_delete(stack_p stack);
void stack_reset(stack_p stack);

void* stack_alloc(stack_p stack, unsigned long long size);
void stack_free(stack_p stack, void* block);

void* stack_expand(stack_p stack, void* block, unsigned long long size);
void stack_shrink(stack_p stack, void* block, unsigned long long size);

/* cellular */

struct __free_cell__
{
    char* start;
    unsigned long long size;

    struct __free_cell__* next;
};
typedef struct __free_cell__ free_cell_t;
typedef struct __free_cell__* free_cell_p;

struct __cellular__
{
    char* data;
    unsigned long long size;
    unsigned long long unit;

    char* end;
    free_cell_p free;

    struct __cellular__* temp;
};
typedef struct __cellular__ cellular_t;
typedef struct __cellular__* cellular_p;

void cellular_init(cellular_p cellular, unsigned long long size, unsigned long long unit);
void cellular_delete(cellular_p cellular);
void cellular_reset(cellular_p cellular);

void* cellular_alloc(cellular_p cellular);
void cellular_free(cellular_p cellular, void* block);

/* heap */

struct __free_block__
{
    char* start;
    unsigned long long size;

    struct __free_block__* next;
};
typedef struct __free_block__ free_block_t;
typedef struct __free_block__* free_block_p;

struct __heap__
{
    char* data;
    unsigned long long size;

    char* end;
    free_block_p free;

    struct __heap__* temp;
};
typedef struct __heap__ heap_t;
typedef struct __heap__* heap_p;

void heap_init(heap_p heap, unsigned long long size);
void heap_delete(heap_p heap);
void heap_reset(heap_p heap);

void* heap_alloc(heap_p heap, unsigned long long size);
void heap_free(heap_p heap, void* block, unsigned long long bsize);

void* heap_expand(heap_p heap, void* block, unsigned long long bsize, unsigned long long size);
void heap_shrink(heap_p heap, void* block, unsigned long long bsize, unsigned long long size);

/* memory */

struct __memory__
{
    stack_t stack;
    heap_t heap;

    cellular_t int_cellular;
    cellular_t float_cellular;
    cellular_t complex_cellular;
    cellular_t str_cellular;
};
typedef struct __memory__ memory_t;
typedef struct __memory__* memory_p;

memory_t memory;

#endif /* __M_MEMORY__ */
