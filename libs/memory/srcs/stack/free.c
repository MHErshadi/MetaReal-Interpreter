/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Frees (block) from (stack)
/*/

#include <memory.h>

void stack_free(stack_p stack, void* block)
{
    while (stack->data > (char*)block || stack->sp <= (char*)block)
        stack = stack->temp;

    stack->sp = (char*)block;
}
