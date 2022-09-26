/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Testing reset fuction
/*/

#include <memory.h>
#include <stdio.h>

#define HEAP_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Heap form");
    puts("Testing reset function\n");

    heap_t heap;
    heap_init(&heap, HEAP_SIZE);

    unsigned long long i;
    for (i = 0; i < HEAP_SIZE * 2; i++)
        heap_alloc(&heap, 8);

    printf("heap before reset: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n\n", heap.data, heap.size, heap.end, heap.free, heap.temp);

    heap_reset(&heap);

    printf("heap after reset: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block after reset: {start=%p, next=%p, size=%llu}\n", heap.free->start, heap.free->next, heap.free->size);

    heap_delete(&heap);
    return 0;
}
