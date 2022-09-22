/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Frees (block) from (stack)
/*/

#include <memory.h>

void stack_free(stack_t stack, const void* block)
{
    const char* cast = block;
    if (stack->data < block && stack->sp > block)
    {
        stack->sp = block;
        return;
    }

    stack_p temp = stack->temp;
    while (temp->data < block && temp->sp > block)
        temp = temp->temp;

    temp->sp = block;
}
