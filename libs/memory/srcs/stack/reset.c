/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Resets (stack) and deletes all temporary stacks
/*/

#include <memory.h>
#include <stdlib.h>

void temp_reset(stack_p temp);

void stack_reset(stack_t stack)
{
    stack->sp = stack->data;

    if (stack->temp)
        temp_reset(stack->temp);
    stack->temp = NULL;
}

void temp_reset(stack_p temp)
{
    if (temp->temp)
        temp_reset(temp->temp);

    free(temp->data);
    free(temp);
}
