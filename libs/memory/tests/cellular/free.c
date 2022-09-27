/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Testing free fuction
/*/

#include <memory.h>
#include <stdio.h>

#define CELLULAR_SIZE 1024 / sizeof(int)
#define CELLULAR_UNIT sizeof(int)

void free_cell_print(free_cell_p cell);

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Cellular form");
    puts("Testing free function\n");

    cellular_t cellular;
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    printf("cellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    free_cell_print(cellular.free);

    void* ptr1 = cellular_alloc(&cellular);
    void* ptr2 = cellular_alloc(&cellular);
    void* ptr3 = cellular_alloc(&cellular);

    printf("\ncellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    free_cell_print(cellular.free);

    cellular_free(&cellular, ptr2);

    printf("\ncellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    free_cell_print(cellular.free);

    cellular_free(&cellular, ptr1);

    printf("\ncellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    free_cell_print(cellular.free);

    cellular_free(&cellular, ptr3);

    printf("\ncellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    free_cell_print(cellular.free);

    cellular_delete(&cellular);
    return 0;
}

void free_cell_print(free_cell_p cell)
{
    printf("free-cell: {start=%p, size=%llu, next=%p}\n", cell->start, cell->size, cell->next);

    if (cell->next)
        free_cell_print(cell->next);
}
