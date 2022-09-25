/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Testing reset fuction
/*/

#include <memory.h>
#include <stdio.h>

#define STACK_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Stack form");
    puts("Testing reset function\n");

    stack_t stack;
    stack_init(&stack, STACK_SIZE);

    stack_alloc(&stack, STACK_SIZE);
    stack_alloc(&stack, STACK_SIZE);

    printf("stack before reset: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack.data, stack.size, stack.sp, stack.temp);

    stack_reset(&stack);

    printf("stack after reset: {data=%p, size=%llu, sp=%p, temp=%p}\n", stack.data, stack.size, stack.sp, stack.temp);

    stack_delete(&stack);
    return 0;
}
