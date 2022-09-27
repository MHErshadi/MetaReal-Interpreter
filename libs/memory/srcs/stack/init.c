/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 *
 * Initializes (stack) with (size) bytes of memory
/*/

#include <memory.h>
#include <stdlib.h>

void stack_init(stack_p stack, unsigned long long size)
{
    stack->data = malloc(size);
    stack->size = size;

    stack->sp = stack->data;

    stack->temp = NULL;
}
