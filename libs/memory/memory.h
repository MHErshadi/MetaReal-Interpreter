/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
/*/

#ifndef __M_MEMORY__
#define __M_MEMORY__

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
void stack_free(stack_t stack, const void* block);

void stack_shrink(stack_t stack, const void* block, unsigned long long size);

void* stack_increase(stack_t stack, void* block, unsigned long long add);

#endif /* __M_MEMORY__ */
