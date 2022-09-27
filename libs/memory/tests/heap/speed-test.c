/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 *
 * Speed test
/*/

#include <memory.h>
#include <stdio.h>
#include <time.h>

#define HEAP_SIZE 1

#define COUNT 10000000

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Heap form");
    puts("Speed test\n");

    heap_t heap;
    heap_init(&heap, HEAP_SIZE);

    clock_t start = clock();

    unsigned long long i;
    for (i = 0; i < COUNT; i++)
        heap_free(&heap, heap_alloc(&heap, 10), 10);

    clock_t end = clock();

    printf("Time: %ld milliseconds\n", end - start);

    heap_delete(&heap);
    return 0;
}
