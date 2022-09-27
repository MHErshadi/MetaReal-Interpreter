/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Deletes (heap) and all temporary heaps with it
/*/

#include <memory.h>
#include <stdlib.h>

void free_block_delete(free_block_p block);
void heap_temp_delete(heap_p temp);

void heap_delete(heap_p heap)
{
    free(heap->data);
    if (heap->free)
        free_block_delete(heap->free);

    if (heap->temp)
        heap_temp_delete(heap->temp);
}

void free_block_delete(free_block_p block)
{
    if (block->next)
        free_block_delete(block->next);

    free(block);
}

void heap_temp_delete(heap_p temp)
{
    if (temp->temp)
        heap_temp_delete(temp->temp);

    free(temp->data);
    if (temp->free)
        free_block_delete(temp->free);
    free(temp);
}
