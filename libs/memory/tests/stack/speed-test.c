/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Speed test
/*/

#include <memory.h>
#include <stdio.h>
#include <time.h>

#define STACK_SIZE 1

#define COUNT 10000000

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Stack form");
    puts("Speed test\n");

    stack_t stack;
    stack_init(stack, STACK_SIZE);

    clock_t start = clock();

    unsigned long long i;
    for (i = 0; i < COUNT; i++)
        stack_free(stack, stack_alloc(stack, 1));

    clock_t end = clock();

    printf("Time: %ld milliseconds\n", end - start);

    stack_delete(stack);
    return 0;
}
