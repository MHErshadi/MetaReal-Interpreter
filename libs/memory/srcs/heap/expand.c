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

void* temp_expand(heap_p heap, void* block, unsigned long long size, unsigned long long alt);

void* heap_expand(heap_p heap, void* block, unsigned long long size)
{
    block = (unsigned long long*)block - 1;
    size += sizeof(unsigned long long);

    unsigned long long alt = heap->size;

    while (heap->data > (char*)block || heap->end <= (char*)block)
        heap = heap->temp;

    if (!heap->free)
        return temp_expand(heap, block, size, alt);

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
            return temp_expand(heap, block, size, alt);

        next->next = malloc(sizeof(free_block_t));
        *next->next = (free_block_t){block, BLOCK_SIZE(block), NULL};

        if (fit->size == size)
        {
            char* res = fit->start;
            BLOCK_SIZE(res) = size;

            if (!prev_fit)
                heap->free = fit->next;
            else
                prev_fit->next = fit->next;
            free(fit);

            unsigned long long i;
            for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
                res[i] = ((char*)block)[i];

            return res + sizeof(unsigned long long);
        }

        char* res = fit->start;
        BLOCK_SIZE(res) = size;

        fit->start += size;
        fit->size -= size;

        unsigned long long i;
        for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
            res[i] = ((char*)block)[i];

        return res + sizeof(unsigned long long);
    }

    if ((char*)block + BLOCK_SIZE(block) == next->start)
    {
        unsigned long long asize = BLOCK_SIZE(block) + next->size;

        if (asize == size)
        {
            if (!prev)
                heap->free = next->next;
            else
                prev->next = next->next;
            free(next);

            BLOCK_SIZE(block) = size;
            return (char*)block + sizeof(unsigned long long);
        }
        if (asize > size)
        {
            next->start += size - BLOCK_SIZE(block);
            next->size = asize - size;

            BLOCK_SIZE(block) = size;
            return (char*)block + sizeof(unsigned long long);
        }

        if (prev && prev->start + prev->size == (char*)block)
        {
            prev->size += BLOCK_SIZE(block) + next->size;

            if (!fit && size <= prev->size)
                fit = prev;
        }
        else
        {
            next->start = block;
            next->size += BLOCK_SIZE(block);
        }
    }
    else
    {
        if (!prev)
        {
            heap->free = malloc(sizeof(free_block_t));
            *heap->free = (free_block_t){block, BLOCK_SIZE(block), next};
        }
        else
        {
            prev->next = malloc(sizeof(free_block_t));
            *prev->next = (free_block_t){block, BLOCK_SIZE(block), next};
        }
    }

    if (fit)
    {
        if (fit->size == size)
        {
            unsigned long long i;
            for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
                fit->start[i] = ((char*)block)[i];

            char* res = fit->start;
            BLOCK_SIZE(res) = size;

            if (!prev_fit)
                heap->free = fit->next;
            else
                prev_fit->next = fit->next;
            free(prev);

            return res + sizeof(unsigned long long);
        }

        unsigned long long i;
        for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
            fit->start[i] = ((char*)block)[i];

        char* res = fit->start;
        BLOCK_SIZE(res) = size;

        fit->start += size;
        fit->size -= size;

        return res + sizeof(unsigned long long);
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
        return temp_expand(heap, block, size, alt);

    if (fit->size == size)
    {
        unsigned long long i;
        for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
            fit->start[i] = ((char*)block)[i];

        char* res = fit->start;
        BLOCK_SIZE(res) = size;

        if (!prev_fit)
            heap->free = fit->next;
        else
            prev_fit->next = fit->next;
        free(prev);

        return res + sizeof(unsigned long long);
    }

    unsigned long long i;
    for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
        fit->start[i] = ((char*)block)[i];

    char* res = fit->start;
    BLOCK_SIZE(res) = size;

    fit->start += size;
    fit->size -= size;

    return res + sizeof(unsigned long long);
}

void* temp_expand(heap_p heap, void* block, unsigned long long size, unsigned long long alt)
{
    heap->free = malloc(sizeof(free_block_t));
    *heap->free = (free_block_t){block, BLOCK_SIZE(block), NULL};

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

        BLOCK_SIZE(heap->temp->data) = size;
        unsigned long long i;
        for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
            heap->temp->data[i] = ((char*)block)[i];

        heap->temp->end = heap->temp->data + heap->temp->size;

        heap->temp->temp = NULL;

        return heap->temp->data + sizeof(unsigned long long);
    }

    if (fit->size == size)
    {
        char* res = fit->start;
        BLOCK_SIZE(res) = size;

        if (!prev)
            heap->free = fit->next;
        else
            prev->next = fit->next;
        free(fit);

        unsigned long long i;
        for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
            res[i] = ((char*)block)[i];

        return res + sizeof(unsigned long long);
    }

    char* res = fit->start;
    BLOCK_SIZE(res) = size;

    unsigned long long i;
    for (i = sizeof(unsigned long long); i < BLOCK_SIZE(block); i++)
        res[i] = ((char*)block)[i];

    fit->start += size;
    fit->size -= size;

    return res + sizeof(unsigned long long);
}
