/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Frees (block) from (cellular)
/*/

#include <memory.h>
#include <stdlib.h>

void cellular_free(cellular_p cellular, void* block)
{
    while (cellular->data > (char*)block || cellular->end <= (char*)block)
        cellular = cellular->temp;

    if (cellular->free)
    {
        free_cell_p prev = NULL;
        free_cell_p next = cellular->free;

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
                next->size += cellular->unit;
                return;
            }

            next->next = malloc(sizeof(free_cell_t));
            *next->next = (free_cell_t){block, cellular->unit, NULL};
            return;
        }

        if (!prev)
        {
            if ((char*)block + cellular->unit == next->start)
            {
                next->start = block;
                next->size += cellular->unit;
                return;
            }

            cellular->free = malloc(sizeof(free_cell_t));
            *cellular->free = (free_cell_t){block, cellular->unit, next};
            return;
        }

        if (prev->start + prev->size == (char*)block)
        {
            if ((char*)block + cellular->unit == next->start)
            {
                prev->size += next->size + cellular->unit;
                prev->next = next->next;

                free(next);
                return;
            }

            prev->size += cellular->unit;
            return;
        }

        if ((char*)block + cellular->unit == next->start)
        {
            next->start = block;
            next->size += cellular->unit;
            return;
        }

        prev->next = malloc(sizeof(free_cell_t));
        *prev->next = (free_cell_t){block, cellular->unit, next};
        return;
    }

    cellular->free = malloc(sizeof(free_cell_t));
    *cellular->free = (free_cell_t){block, 1, NULL};
}
