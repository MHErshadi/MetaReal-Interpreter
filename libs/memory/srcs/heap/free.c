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

void heap_free(heap_p heap, void* block, unsigned long long bsize)
{
    while (heap->data > (char*)block || heap->end <= (char*)block)
        heap = heap->temp;

    if (!heap->free)
    {
        heap->free = malloc(sizeof(free_block_t));
        *heap->free = (free_block_t){block, bsize, NULL};
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
            next->size += bsize;
            return;
        }

        next->next = malloc(sizeof(free_block_t));
        *next->next = (free_block_t){block, bsize, NULL};
        return;
    }

    if (!prev)
    {
        if ((char*)block + bsize == next->start)
        {
            next->start = block;
            next->size += bsize;
            return;
        }

        heap->free = malloc(sizeof(free_block_t));
        *heap->free = (free_block_t){block, bsize, next};
        return;
    }

    if (prev->start + prev->size == (char*)block)
    {
        if ((char*)block + bsize == next->start)
        {
            prev->next = next->next;
            prev->size += bsize + next->size;

            free(next);
            return;
        }

        prev->size += bsize;
        return;
    }

    if ((char*)block + bsize == next->start)
    {
        next->start = block;
        next->size += bsize;
        return;
    }

    prev->next = malloc(sizeof(free_block_t));
    *prev->next = (free_block_t){block, bsize, next};
    return;
}
