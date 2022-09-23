/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Testing init fuction
/*/

#include <memory.h>
#include <stdio.h>

#define STACK_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Stack form");
    puts("Testing init function\n");

    stack_t stack;

    printf("stack before init: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack->data, stack->size, stack->sp, stack->temp);

    stack_init(stack, STACK_SIZE);

    printf("stack after init: {data=%p, size=%llu, sp=%p, temp=%p}\n", stack->data, stack->size, stack->sp, stack->temp);

    stack_delete(stack);
    return 0;
}
