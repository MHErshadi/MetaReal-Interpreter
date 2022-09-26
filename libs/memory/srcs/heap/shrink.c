/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Shrinks (block) down to (size) bytes
/*/

#include <memory.h>
#include <stdlib.h>

void heap_shrink(heap_p heap, void* block, unsigned long long bsize, unsigned long long size)
{
    while (heap->data > (char*)block || heap->end <= (char*)block)
        heap = heap->temp;

    if (!heap->free)
    {
        heap->free = malloc(sizeof(free_block_t));
        *heap->free = (free_block_t){(char*)block + size, bsize - size, NULL};
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
        next->next = malloc(sizeof(free_block_t));
        *next->next = (free_block_t){(char*)block + size, bsize - size, NULL};
        return;
    }

    if ((char*)block + bsize == next->start)
    {
        next->start = (char*)block + size;
        next->size += bsize - size;
        return;
    }

    if (!prev)
    {
        heap->free = malloc(sizeof(free_block_t));
        *heap->free = (free_block_t){(char*)block + size, bsize - size, next};
        return;
    }

    prev->next = malloc(sizeof(free_block_t));
    *prev->next = (free_block_t){(char*)block + size, bsize - size, next};
}
