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
typedef struct __stack__ stack_t[1];
typedef struct __stack__* stack_p;

void stack_init(stack_t stack, unsigned long long size);
void stack_delete(stack_t stack);
void stack_reset(stack_t stack);

void* stack_alloc(stack_t stack, unsigned long long size);
void stack_free(stack_t stack, void* block);

void stack_shrink(stack_t stack, void* block, unsigned long long size);

void* stack_increase(stack_t stack, void* block, unsigned long long add);

/* cellular */

struct __cell__
{
    char* start;
    unsigned long long size;

    struct __cell__* next;
};
typedef struct __cell__ cell_t;
typedef struct __cell__* cell_p;

struct __cellular__
{
    char* data;
    unsigned long long size;
    unsigned long long unit;

    char* end;
    cell_p free;

    struct __cellular__* temp;
};
typedef struct __cellular__ cellular_t[1];
typedef struct __cellular__* cellular_p;

void cellular_init(cellular_t cellular, unsigned long long size, unsigned long long unit);
void cellular_delete(cellular_t cellular);
void cellular_reset(cellular_t cellular);

void* cellular_alloc(cellular_t cellular);
void cellular_free(cellular_t cellular, void* block);

#endif /* __M_MEMORY__ */
