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

void* stack_increase(stack_t stack, void* block, unsigned long long add)
{
    unsigned long long fsize = stack->size - (stack->sp - stack->data);

    if (fsize < add)
    {
        unsigned long long osize = stack->sp - (char*)block;
        unsigned long long size = osize + add;

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

                char* cast = block;
                unsigned long long i;
                for (i = 0; i < osize; i++)
                    temp->data[i] = cast[i];

                storage->temp = temp;
                return temp->data;
            }

            void* block = storage->sp;

            char* cast = block;
            unsigned long long i;
            for (i = 0; i < osize; i++)
                storage->sp[i] = cast[i];

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

        char* cast = block;
        unsigned long long i;
        for (i = 0; i < osize; i++)
            temp->data[i] = cast[i];

        stack->temp = temp;
        return temp->data;
    }

    stack->sp += add;
    return block;
}
