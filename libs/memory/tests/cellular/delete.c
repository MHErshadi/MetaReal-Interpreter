/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 * 
 * Testing delete fuction
/*/

#include <memory.h>
#include <stdio.h>

#define CELLULAR_SIZE 1024 / sizeof(int)
#define CELLULAR_UNIT sizeof(int)

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Cellular form");
    puts("Testing delete function\n");

    cellular_t cellular;
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    printf("cellular before delete: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    printf("free-cell before delete: {start=%p, size=%llu, next=%p}\n\n", cellular.free->start, cellular.free->size, cellular.free->next);

    cellular_delete(&cellular);

    printf("cellular after delete: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);

    return 0;
}
