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

void cellular_free(cellular_t cellular, void* block)
{
    if (cellular->data <= (char*)block && cellular->end > (char*)block)
    {
        if (cellular->free)
        {
            cell_p prev = NULL;
            cell_p next = cellular->free;

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

                next->next = malloc(sizeof(cell_t));
                *next->next = (cell_t){block, cellular->unit, NULL};
                return;
            }

            if (!prev)
            {
                if ((char*)block + cellular->unit == next->start)
                {
                    next->start -= cellular->unit;
                    next->size += cellular->unit;
                    return;
                }

                cellular->free = malloc(sizeof(cell_t));
                *cellular->free = (cell_t){block, cellular->unit, next};
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
                next->start -= cellular->unit;
                next->size += cellular->unit;
                return;
            }

            prev->next = malloc(sizeof(cell_t));
            *prev->next = (cell_t){block, cellular->unit, next};
            return;
        }

        cellular->free = malloc(sizeof(cell_t));
        *cellular->free = (cell_t){block, 1, NULL};
        return;
    }

    cellular_p temp = cellular->temp;
    while (cellular->data <= (char*)block && cellular->end > (char*)block)
        temp = temp->temp;

    if (temp->free)
    {
        cell_p prev = NULL;
        cell_p next = temp->free;

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
                next->size += temp->unit;
                return;
            }

            next->next = malloc(sizeof(cell_t));
            *next->next = (cell_t){block, temp->unit, NULL};
            return;
        }

        if (!prev)
        {
            if ((char*)block + temp->unit == next->start)
            {
                next->start -= temp->unit;
                next->size += temp->unit;
                return;
            }

            temp->free = malloc(sizeof(cell_t));
            *temp->free = (cell_t){block, temp->unit, next};
            return;
        }

        if (prev->start + prev->size == (char*)block)
        {
            if ((char*)block + temp->unit == next->start)
            {
                prev->size += next->size + temp->unit;
                prev->next = next->next;

                free(next);
                return;
            }

            prev->size += temp->unit;
            return;
        }

        if ((char*)block + temp->unit == next->start)
        {
            next->start -= temp->unit;
            next->size += temp->unit;
            return;
        }

        prev->next = malloc(sizeof(cell_t));
        *prev->next = (cell_t){block, temp->unit, next};
        return;
    }

    temp->free = malloc(sizeof(cell_t));
    *temp->free = (cell_t){block, 1, NULL};
}
