/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Shrinks (block) down to (size) bytes
/*/

#include <memory.h>

void stack_shrink(stack_p stack, void* block, unsigned long long size)
{
    while (stack->data > (char*)block || stack->sp <= (char*)block)
        stack = stack->temp;

    stack->sp = (char*)block + size;
}
