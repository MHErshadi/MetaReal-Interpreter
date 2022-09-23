/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Cellular form
 * 
 * Speed test
/*/

#include <memory.h>
#include <stdio.h>
#include <time.h>

#define CELLULAR_SIZE 10000000
#define CELLULAR_UNIT sizeof(int)

#define COUNT 10000000

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Cellular form");
    puts("Speed test\n");

    cellular_t cellular;
    cellular_init(cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    clock_t start = clock();

    unsigned long long i;
    for (i = 0; i < COUNT; i++)
        cellular_alloc(cellular);

    clock_t end = clock();

    printf("Time: %ld milliseconds\n", end - start);

    cellular_delete(cellular);
    return 0;
}
