/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 *
 * Expands (block) up to (size) bytes
/*/

#include <memory.h>
#include <stdlib.h>

void* temp_expand(heap_p heap, void* block, unsigned long long bsize, unsigned long long size, unsigned long long alt);

void* heap_expand(heap_p heap, void* block, unsigned long long bsize, unsigned long long size)
{
    unsigned long long alt = heap->size;

    while (heap->data > (char*)block || heap->end <= (char*)block)
        heap = heap->temp;

    if (!heap->free)
        return temp_expand(heap, block, bsize, size, alt);

    free_block_p prev = NULL;
    free_block_p next = heap->free;

    free_block_p prev_fit = NULL;
    free_block_p fit = size <= next->size ? next : NULL;

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

        if (!fit && size < next->size)
        {
            prev_fit = prev;
            fit = next;
        }
    }

    if (last)
    {
        if (!fit)
            return temp_expand(heap, block, bsize, size, alt);

        next->next = malloc(sizeof(free_block_t));
        *next->next = (free_block_t){block, bsize, NULL};

        if (fit->size == size)
        {
            char* res = fit->start;

            if (!prev_fit)
                heap->free = fit->next;
            else
                prev_fit->next = fit->next;
            free(fit);

            unsigned long long i;
            for (i = 0; i < bsize; i++)
                res[i] = ((char*)block)[i];

            return res;
        }

        char* res = fit->start;

        fit->start += size;
        fit->size -= size;

        unsigned long long i;
        for (i = 0; i < bsize; i++)
            res[i] = ((char*)block)[i];

        return res;
    }

    if ((char*)block + bsize == next->start)
    {
        unsigned long long asize = bsize + next->size;

        if (asize == size)
        {
            if (!prev)
                heap->free = next->next;
            else
                prev->next = next->next;
            free(next);

            return block;
        }
        if (asize > size)
        {
            next->start += size - bsize;
            next->size = asize - size;

            return block;
        }

        if (prev && prev->start + prev->size == (char*)block)
        {
            prev->size += bsize + next->size;

            if (!fit && size <= prev->size)
                fit = prev;
        }
        else
        {
            next->start = block;
            next->size += bsize;
        }
    }
    else
    {
        if (!prev)
        {
            heap->free = malloc(sizeof(free_block_t));
            *heap->free = (free_block_t){block, bsize, next};
        }
        else
        {
            prev->next = malloc(sizeof(free_block_t));
            *prev->next = (free_block_t){block, bsize, next};
        }
    }

    if (fit)
    {
        if (fit->size == size)
        {
            unsigned long long i;
            for (i = 0; i < bsize; i++)
                fit->start[i] = ((char*)block)[i];

            void* res = fit->start;

            if (!prev_fit)
                heap->free = fit->next;
            else
                prev_fit->next = fit->next;
            free(fit);

            return res;
        }

        unsigned long long i;
        for (i = 0; i < bsize; i++)
            fit->start[i] = ((char*)block)[i];

        void* res = fit->start;

        fit->start += size;
        fit->size -= size;

        return res;
    }

    prev_fit = NULL;
    fit = next;

    while (fit->size < size)
    {
        if (!fit->next)
        {
            last = 1;
            break;
        }

        prev_fit = fit;
        fit = fit->next;
    }

    if (last)
        return temp_expand(heap, block, bsize, size, alt);

    if (fit->size == size)
    {
        unsigned long long i;
        for (i = 0; i < bsize; i++)
            fit->start[i] = ((char*)block)[i];

        void* res = fit->start;

        if (!prev_fit)
            heap->free = fit->next;
        else
            prev_fit->next = fit->next;
        free(prev);

        return res;
    }

    unsigned long long i;
    for (i = 0; i < bsize; i++)
        fit->start[i] = ((char*)block)[i];

    void* res = fit->start;

    fit->start += size;
    fit->size -= size;

    return res;
}

void* temp_expand(heap_p heap, void* block, unsigned long long bsize, unsigned long long size, unsigned long long alt)
{
    heap->free = malloc(sizeof(free_block_t));
    *heap->free = (free_block_t){block, bsize, NULL};

    free_block_p prev = NULL;
    free_block_p fit = NULL;

    while (heap->temp)
    {
        heap = heap->temp;

        if (!heap->free)
            continue;

        prev = NULL;
        fit = heap->free;
        while (fit && fit->size < size)
        {
            prev = fit;
            fit = fit->next;
        }

        if (fit)
            break;
    }

    if (!fit)
    {
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

        unsigned long long i;
        for (i = 0; i < bsize; i++)
            heap->temp->data[i] = ((char*)block)[i];

        heap->temp->end = heap->temp->data + heap->temp->size;

        heap->temp->temp = NULL;

        return heap->temp->data;
    }

    if (fit->size == size)
    {
        char* res = fit->start;

        if (!prev)
            heap->free = fit->next;
        else
            prev->next = fit->next;
        free(fit);

        unsigned long long i;
        for (i = 0; i < bsize; i++)
            res[i] = ((char*)block)[i];

        return res;
    }

    char* res = fit->start;

    unsigned long long i;
    for (i = 0; i < bsize; i++)
        res[i] = ((char*)block)[i];

    fit->start += size;
    fit->size -= size;

    return res;
}
