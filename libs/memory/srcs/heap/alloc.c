/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Allocates (size) bytes from (heap) if exist, allocates it in temporary heaps otherwise
/*/

#include <memory.h>
#include <stdlib.h>

void* temp_alloc(heap_p heap, unsigned long long size, unsigned long long alt);

void* heap_alloc(heap_p heap, unsigned long long size)
{
    return temp_alloc(heap, size, heap->size);
}

void* temp_alloc(heap_p heap, unsigned long long size, unsigned long long alt)
{
    if (!heap->free)
    {
        if (heap->temp)
            return temp_alloc(heap->temp, size, alt);

        heap->temp = malloc(sizeof(heap_t));

        if (alt <= size)
        {
            heap->temp->data = malloc(size);
            heap->temp->size = size;

            heap->temp->free = NULL;
        }
        else
        {
            heap->temp->data = malloc(alt);
            heap->temp->size = alt;

            heap->temp->free = malloc(sizeof(free_block_t));
            *heap->temp->free = (free_block_t){heap->temp->data + size, alt - size, NULL};
        }

        heap->temp->end = heap->temp->data + heap->temp->size;

        heap->temp->temp = NULL;

        return heap->temp->data;
    }

    free_block_p prev = NULL;
    free_block_p fit = heap->free;

    char new = 0;
    while (fit->size < size)
    {
        if (!fit->next)
        {
            new = 1;
            break;
        }

        prev = fit;
        fit = fit->next;
    }

    if (new)
    {
        if (heap->temp)
            return temp_alloc(heap->temp, size, alt);

        heap->temp = malloc(sizeof(heap_t));

        if (alt < size)
        {
            heap->temp->data = malloc(size);
            heap->temp->size = size;

            heap->temp->free = NULL;
        }
        else
        {
            heap->temp->data = malloc(alt);
            heap->temp->size = alt;

            heap->temp->free = malloc(sizeof(free_block_t));
            *heap->temp->free = (free_block_t){heap->temp->data + size, alt - size, NULL};
        }

        heap->temp->end = heap->temp->data + heap->temp->size;

        heap->temp->temp = NULL;

        return heap->temp->data;
    }

    if (fit->size == size)
    {
        void* block = fit->start;

        if (!prev)
            heap->free = fit->next;
        else
            prev->next = fit->next;
        free(fit);

        return block;
    }

    void* block = fit->start;

    fit->start += size;
    fit->size -= size;

    return block;
}
