/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 *
 * Testing delete fuction
/*/

#include <memory.h>
#include <stdio.h>

#define STACK_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Stack form");
    puts("Testing delete function\n");

    stack_t stack;
    stack_init(&stack, STACK_SIZE);

    printf("stack before delete: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack.data, stack.size, stack.sp, stack.temp);

    stack_delete(&stack);

    printf("stack after delete: {data=%p, size=%llu, sp=%p, temp=%p}\n", stack.data, stack.size, stack.sp, stack.temp);

    return 0;
}
