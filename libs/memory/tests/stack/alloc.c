/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Testing alloc fuction
/*/

#include <memory.h>
#include <stdio.h>
#include <string.h>

#define STACK_SIZE 1024

struct vector
{
    int x;
    int y;
};

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Stack form");
    puts("Testing alloc function\n");

    stack_t stack;
    stack_init(stack, STACK_SIZE);

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack->data, stack->size, stack->sp, stack->temp);

    char* text = stack_alloc(stack, 12);
    strncpy(text, "Hello World", 12);

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack->data, stack->size, stack->sp, stack->temp);

    struct vector* vec = stack_alloc(stack, sizeof(struct vector));
    vec->x = 10;
    vec->y = 20;

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack->data, stack->size, stack->sp, stack->temp);

    int* age = stack_alloc(stack, sizeof(int));
    *age = 57;

    printf("stack: {data=%p, size=%llu, sp=%p, temp=%p}\n\n", stack->data, stack->size, stack->sp, stack->temp);

    printf("stack data character form:\n");
    unsigned long long i;
    for (i = 0; i < STACK_SIZE; i++)
        printf("%c", stack->data[i]);

    printf("\n\nstack data int form:\n");
    for (i = 0; i < STACK_SIZE; i++)
        printf("%d ", stack->data[i]);
    printf("\n");

    stack_delete(stack);
    return 0;
}
