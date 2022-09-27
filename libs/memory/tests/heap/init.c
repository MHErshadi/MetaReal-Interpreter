/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 *
 * Testing init fuction
/*/

#include <memory.h>
#include <stdio.h>

#define HEAP_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Heap form");
    puts("Testing init function\n");

    heap_t heap;

    printf("heap before init: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n\n", heap.data, heap.size, heap.end, heap.free, heap.temp);

    heap_init(&heap, HEAP_SIZE);

    printf("heap after init: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block after init: {start=%p, next=%p, size=%llu}\n", heap.free->start, heap.free->next, heap.free->size);

    heap_delete(&heap);
    return 0;
}
