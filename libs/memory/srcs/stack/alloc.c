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

void* stack_alloc(stack_t stack, unsigned long long size)
{
    unsigned long long fsize = stack->size - (stack->sp - stack->data);

    if (fsize < size)
    {
        if (stack->temp)
        {
            stack_p storage = stack->temp;
            fsize = storage->size - (storage->sp - storage->data);

            char new = 0;
            while (fsize < size)
            {
                if (!storage->temp)
                {
                    new = 1;
                    break;
                }

                storage = storage->temp;
                fsize = storage->size - (storage->sp - storage->data);
            }

            if (new)
            {
                stack_p temp = malloc(sizeof(struct __stack__));

                if (stack->size < size)
                {
                    temp->data = malloc(size);
                    temp->size = size;

                    temp->sp = stack->data + size;
                }
                else
                {
                    temp->data = malloc(stack->size);
                    temp->size = stack->size;

                    temp->sp = stack->data + stack->size;
                }

                temp->temp = NULL;

                storage->temp = temp;
                return temp->data;
            }

            void* block = storage->sp;

            storage->sp += size;

            return block;
        }

        stack_p temp = malloc(sizeof(struct __stack__));

        if (stack->size < size)
        {
            temp->data = malloc(size);
            temp->size = size;

            temp->sp = stack->data + size;
        }
        else
        {
            temp->data = malloc(stack->size);
            temp->size = stack->size;

            temp->sp = stack->data + stack->size;
        }

        temp->temp = NULL;

        stack->temp = temp;
        return temp->data;
    }

    void* block = stack->sp;

    stack->sp += size;

    return block;
}
