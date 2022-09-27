/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 *
 * Testing alloc fuction
/*/

#include <memory.h>
#include <stdio.h>

#define CELLULAR_SIZE 1024 / sizeof(int)
#define CELLULAR_UNIT sizeof(int)

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Cellular form");
    puts("Testing alloc function\n");

    cellular_t cellular;
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    printf("cellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    printf("free-cell: {start=%p, size=%llu, next=%p}\n\n", cellular.free->start, cellular.free->size, cellular.free->next);

    int* int1 = cellular_alloc(&cellular);
    *int1 = 100;

    printf("cellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    printf("free-cell: {start=%p, size=%llu, next=%p}\n\n", cellular.free->start, cellular.free->size, cellular.free->next);

    int* int2 = cellular_alloc(&cellular);
    *int2 = 200;

    printf("cellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    printf("free-cell: {start=%p, size=%llu, next=%p}\n\n", cellular.free->start, cellular.free->size, cellular.free->next);

    cellular_free(&cellular, int1);

    int* int3 = cellular_alloc(&cellular);
    *int3 = 300;

    printf("cellular: {data=%p, size=%llu, unit=%llu, end=%p, free=%p, temp=%p}\n", cellular.data, cellular.size, cellular.unit, cellular.end, cellular.free, cellular.temp);
    printf("free-cell: {start=%p, size=%llu, next=%p}\n\n", cellular.free->start, cellular.free->size, cellular.free->next);

    printf("cellular data int form:\n");
    unsigned long long i;
    for (i = 0; i < CELLULAR_SIZE; i++)
        printf("%d ", ((int*)cellular.data)[i]);
    printf("\n");

    cellular_delete(&cellular);
    return 0;
}
