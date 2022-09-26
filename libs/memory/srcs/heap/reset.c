/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Resets (heap) and deletes all temporary heaps
/*/

#include <memory.h>
#include <stdlib.h>

void free_block_reset(free_block_p block);
void temp_reset(heap_p temp);

void heap_reset(heap_p heap)
{

    if (heap->free)
    {
        if (heap->free->next)
            free_block_reset(heap->free->next);
    }
    else
        heap->free = malloc(sizeof(free_block_t));

    *heap->free = (free_block_t){heap->data, heap->size, NULL};

    if (heap->temp)
    {
        temp_reset(heap->temp);
        heap->temp = NULL;
    }
}

void free_block_reset(free_block_p block)
{
    if (block->next)
        free_block_reset(block->next);

    free(block);
}

void temp_reset(heap_p temp)
{
    if (temp->temp)
        temp_reset(temp->temp);

    free(temp->data);
    if (temp->free)
        free_block_reset(temp->free);
    free(temp);
}
