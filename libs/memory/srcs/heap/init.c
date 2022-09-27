/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 *
 * Initializes (heap) with (size) bytes of memory
/*/

#include <memory.h>
#include <stdlib.h>

void heap_init(heap_p heap, unsigned long long size)
{
    heap->data = malloc(size);
    heap->size = size;

    heap->end = heap->data + size;
    heap->free = malloc(sizeof(free_block_t));
    *heap->free = (free_block_t){heap->data, size, NULL};

    heap->temp = NULL;
}
