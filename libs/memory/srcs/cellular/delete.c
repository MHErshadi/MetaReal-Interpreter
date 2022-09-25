/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Deletes (cellular) and all temporary cellulars with it
/*/

#include <memory.h>
#include <stdlib.h>

void free_cell_delete(free_cell_p cell);
void temp_delete(cellular_p temp);

void cellular_delete(cellular_p cellular)
{
    free(cellular->data);
    if (cellular->free)
        free_cell_delete(cellular->free);

    if (cellular->temp)
        temp_delete(cellular->temp);
}

void free_cell_delete(free_cell_p cell)
{
    if (cell->next)
        free_cell_delete(cell->next);

    free(cell);
}

void temp_delete(cellular_p temp)
{
    if (temp->temp)
        temp_delete(temp->temp);

    free(temp->data);
    if (temp->free)
        free_cell_delete(temp->free);
    free(temp);
}
