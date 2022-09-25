/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Testing increase fuction
/*/

#include <memory.h>
#include <stdio.h>
#include <string.h>

#define STACK_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Stack form");
    puts("Testing increase function\n");

    stack_t stack;
    stack_init(&stack, STACK_SIZE);

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack.data, stack.size, stack.sp, stack.temp);

    void* ptr = stack_alloc(&stack, 20);

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack.data, stack.size, stack.sp, stack.temp);

    ptr = stack_increase(&stack, ptr, 58);

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n", stack.data, stack.size, stack.sp, stack.temp);

    stack_delete(&stack);
    return 0;
}
