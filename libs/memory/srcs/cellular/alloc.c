/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Allocates a cell from (cellular) if exist, allocates it in temporary cellulars otherwise
/*/

#include <memory.h>
#include <stdlib.h>

void* cellular_alloc(cellular_p cellular)
{
    if (!cellular->free)
    {
        if (cellular->temp)
        {
            cellular = cellular->temp;

            char new = 0;
            while (!cellular->free)
            {
                if (!cellular->temp)
                {
                    new = 1;
                    break;
                }

                cellular = cellular->temp;
            }

            if (new)
            {
                cellular_p temp = malloc(sizeof(cellular_t));

                unsigned long long alloc = cellular->size * cellular->unit;

                temp->data = malloc(alloc);
                temp->size = cellular->size;
                temp->unit = cellular->unit;

                temp->end = temp->data + alloc;
                temp->free = malloc(sizeof(free_cell_t));
                *temp->free = (free_cell_t){temp->data + cellular->unit, alloc - cellular->unit, NULL};

                temp->temp = NULL;

                cellular->temp = temp;
                return temp->data;
            }

            void* block = cellular->free->start;

            if (cellular->free->size == cellular->unit)
            {
                free_cell_p new = cellular->free->next;

                free(cellular->free);
                cellular->free = new;
            }
            else
            {
                cellular->free->start += cellular->unit;
                cellular->free->size -= cellular->unit;
            }

            return block;
        }

        cellular_p temp = malloc(sizeof(cellular_t));

        unsigned long long alloc = cellular->size * cellular->unit;

        temp->data = malloc(alloc);
        temp->size = cellular->size;
        temp->unit = cellular->unit;

        temp->end = temp->data + alloc;
        temp->free = malloc(sizeof(free_cell_t));
        *temp->free = (free_cell_t){temp->data + cellular->unit, alloc - cellular->unit, NULL};

        temp->temp = NULL;

        cellular->temp = temp;
        return temp->data;
    }

    void* block = cellular->free->start;

    if (cellular->free->size == cellular->unit)
    {
        free_cell_p new = cellular->free->next;

        free(cellular->free);
        cellular->free = new;
    }
    else
    {
        cellular->free->start += cellular->unit;
        cellular->free->size -= cellular->unit;
    }

    return block;
}
