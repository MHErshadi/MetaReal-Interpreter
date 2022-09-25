/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 * 
 * Testing reset fuction
/*/

#include <memory.h>
#include <stdio.h>

#define CELLULAR_SIZE 1024 / sizeof(int)
#define CELLULAR_UNIT sizeof(int)

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Cellular form");
    puts("Testing reset function\n");

    cellular_t cellular;
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    unsigned long long i;
    for (i = 0; i < CELLULAR_SIZE * 2; i++)
        cellular_alloc(&cellular);

    printf("cellular before reset: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);

    cellular_reset(&cellular);

    printf("cellular after reset: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    printf("free-cell after reset: {start=%p, size=%llu, next=%p}\n", cellular.free->start, cellular.free->size, cellular.free->next);

    cellular_delete(&cellular);
    return 0;
}
