/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 *
 * Increases (block) size by (add) bytes, stores it in temporary stacks if needed
/*/

#include <memory.h>
#include <stdlib.h>

void* stack_increase(stack_p stack, void* block, unsigned long long add)
{
    unsigned long long alt = stack->size;

    while (stack->data > (char*)block || stack->sp <= (char*)block)
        stack = stack->temp;

    unsigned long long fsize = stack->size - (stack->sp - stack->data);

    if (fsize < add)
    {
        unsigned long long osize = stack->sp - (char*)block;
        unsigned long long size = osize + add;

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

                stack->temp->size = alt < size ? size : alt;
                stack->temp->data = malloc(stack->temp->size);

                stack->temp->sp = stack->data + stack->temp->size;

                stack->temp->temp = NULL;

                unsigned long long i;
                for (i = 0; i < osize; i++)
                    stack->temp->data[i] = ((char*)block)[i];

                return stack->temp->data;
            }

            void* block = stack->sp;

            unsigned long long i;
            for (i = 0; i < osize; i++)
                stack->sp[i] = ((char*)block)[i];

            stack->sp += size;

            return block;
        }

        stack->temp = malloc(sizeof(stack_t));

        stack->temp->size = alt < size ? size : alt;
        stack->temp->data = malloc(stack->temp->size);

        stack->temp->sp = stack->data + stack->temp->size;

        stack->temp->temp = NULL;

        unsigned long long i;
        for (i = 0; i < osize; i++)
            stack->temp->data[i] = ((char*)block)[i];

        return stack->temp->data;
    }

    stack->sp += add;
    return block;
}
