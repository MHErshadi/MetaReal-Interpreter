/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Frees (block) from (stack)
/*/

#include <memory.h>

void stack_free(stack_t stack, void* block)
{
    if (stack->data <= (char*)block && stack->sp >= (char*)block)
    {
        stack->sp = (char*)block;
        return;
    }

    stack_p temp = stack->temp;
    while (temp->data <= (char*)block && temp->sp >= (char*)block)
        temp = temp->temp;

    temp->sp = (char*)block;
}
