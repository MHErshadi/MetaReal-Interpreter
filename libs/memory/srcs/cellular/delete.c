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

void cell_delete(cell_p cell);
void temp_delete(cellular_p temp);

void cellular_delete(cellular_t cellular)
{
    free(cellular->data);
    if (cellular->free)
        cell_delete(cellular->free);

    if (cellular->temp)
        temp_delete(cellular->temp);
}

void cell_delete(cell_p cell)
{
    if (cell->next)
        cell_delete(cell->next);

    free(cell);
}

void temp_delete(cellular_p temp)
{
    if (temp->temp)
        temp_delete(temp->temp);

    free(temp->data);
    if (temp->free)
        cell_delete(temp->free);
    free(temp);
}
