/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Frees (block) from (heap)
/*/

#include <memory.h>
#include <stdlib.h>

void heap_free(heap_p heap, void* block)
{
    block = (unsigned long long*)block - 1;

    while (heap->data > (char*)block || heap->end <= (char*)block)
        heap = heap->temp;

    if (!heap->free)
    {
        heap->free = malloc(sizeof(free_block_t));
        *heap->free = (free_block_t){block, BLOCK_SIZE(block), NULL};
        return;
    }

    free_block_p prev = NULL;
    free_block_p next = heap->free;

    char last = 0;
    while (next->start < (char*)block)
    {
        if (!next->next)
        {
            last = 1;
            break;
        }

        prev = next;
        next = next->next;
    }

    if (last)
    {
        if (next->start + next->size == (char*)block)
        {
            next->size += BLOCK_SIZE(block);
            return;
        }

        next->next = malloc(sizeof(free_block_t));
        *next->next = (free_block_t){block, BLOCK_SIZE(block), NULL};
        return;
    }

    if (!prev)
    {
        if ((char*)block + BLOCK_SIZE(block) == next->start)
        {
            next->start = block;
            next->size += BLOCK_SIZE(block);
            return;
        }

        heap->free = malloc(sizeof(free_block_t));
        *heap->free = (free_block_t){block, BLOCK_SIZE(block), next};
        return;
    }

    if (prev->start + prev->size == (char*)block)
    {
        if ((char*)block + BLOCK_SIZE(block) == next->start)
        {
            prev->next = next->next;
            prev->size += BLOCK_SIZE(block) + next->size;

            free(next);
            return;
        }

        prev->size += BLOCK_SIZE(block);
        return;
    }

    if ((char*)block + BLOCK_SIZE(block) == next->start)
    {
        next->start = block;
        next->size += BLOCK_SIZE(block);
        return;
    }

    prev->next = malloc(sizeof(free_block_t));
    *prev->next = (free_block_t){block, BLOCK_SIZE(block), next};
    return;
}
