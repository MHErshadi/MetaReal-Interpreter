/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Initializes (cellular) with (size * unit) bytes of memory
/*/

#include <memory.h>
#include <stdlib.h>

void cellular_init(cellular_p cellular, unsigned long long size, unsigned long long unit)
{
    unsigned long long alloc = size * unit;

    cellular->data = malloc(alloc);
    cellular->size = size;
    cellular->unit = unit;

    cellular->end = cellular->data + alloc;
    cellular->free = malloc(sizeof(free_cell_t));
    *cellular->free = (free_cell_t){cellular->data, alloc, NULL};

    cellular->temp = NULL;
}
