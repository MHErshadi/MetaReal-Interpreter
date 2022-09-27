/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Resets (cellular) and deletes all temporary cellulars
/*/

#include <memory.h>
#include <stdlib.h>

void free_cell_reset(free_cell_p cell);
void cellular_temp_reset(cellular_p temp);

void cellular_reset(cellular_p cellular)
{
    if (cellular->free)
    {
        if (cellular->free->next)
            free_cell_reset(cellular->free->next);
    }
    else
        cellular->free = malloc(sizeof(free_cell_t));

    *cellular->free = (free_cell_t){cellular->data, cellular->size * cellular->unit, NULL};

    if (cellular->temp)
    {
        cellular_temp_reset(cellular->temp);
        cellular->temp = NULL;
    }
}

void free_cell_reset(free_cell_p cell)
{
    if (cell->next)
        free_cell_reset(cell->next);

    free(cell);
}

void cellular_temp_reset(cellular_p temp)
{
    if (temp->temp)
        cellular_temp_reset(temp->temp);

    free(temp->data);
    if (temp->free)
        free_cell_reset(temp->free);
    free(temp);
}
