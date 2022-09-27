/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 *
 * Allocates (size) bytes from (stack) if exist, allocates it in temporary stacks otherwise
/*/

#include <memory.h>
#include <stdlib.h>

void* stack_alloc(stack_p stack, unsigned long long size)
{
    unsigned long long fsize = stack->size - (stack->sp - stack->data);

    if (fsize < size)
    {
        if (stack->temp)
        {
            stack = stack->temp;
            fsize = stack->size - (stack->sp - stack->data);

            char new = 0;
            while (fsize < size)
            {
                if (!stack->temp)
                {
                    new = 1;
                    break;
                }

                stack = stack->temp;
                fsize = stack->size - (stack->sp - stack->data);
            }

            if (new)
            {
                stack->temp = malloc(sizeof(stack_t));

                stack->temp->size = stack->size < size ? size : stack->size;
                stack->temp->data = malloc(stack->temp->size);

                stack->temp->sp = stack->data + stack->temp->size;

                stack->temp->temp = NULL;

                return stack->temp->data;
            }

            void* block = stack->sp;

            stack->sp += size;

            return block;
        }

        stack->temp = malloc(sizeof(stack_t));

        stack->temp->size = stack->size < size ? size : stack->size;
        stack->temp->data = malloc(stack->temp->size);

        stack->temp->sp = stack->data + stack->temp->size;

        stack->temp->temp = NULL;

        return stack->temp->data;
    }

    void* block = stack->sp;

    stack->sp += size;

    return block;
}
