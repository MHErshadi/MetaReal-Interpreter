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

void cell_reset(cell_p cell);
void temp_reset(cellular_p temp);

void cellular_reset(cellular_t cellular)
{
    if (cellular->free)
    {
        if (cellular->free->next)
            cell_reset(cellular->free->next);
    }
    else
        cellular->free = malloc(sizeof(cell_t));

    *cellular->free = (cell_t){cellular->data, cellular->size * cellular->unit, NULL};

    if (cellular->temp)
    {
        temp_reset(cellular->temp);
        cellular->temp = NULL;
    }
}

void cell_reset(cell_p cell)
{
    if (cell->next)
        cell_reset(cell->next);

    free(cell);
}

void temp_reset(cellular_p temp)
{
    if (temp->temp)
        temp_reset(temp->temp);

    free(temp->data);
    if (temp->free)
        cell_reset(temp->free);
    free(temp);
}
