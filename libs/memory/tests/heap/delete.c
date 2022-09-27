/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 *
 * Testing delete fuction
/*/

#include <memory.h>
#include <stdio.h>

#define HEAP_SIZE 1024

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Heap form");
    puts("Testing delete function\n");

    heap_t heap;
    heap_init(&heap, HEAP_SIZE);

    printf("heap before delete: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block before delete: {start=%p, next=%p, size=%llu}\n\n", heap.free->start, heap.free->next, heap.free->size);

    heap_delete(&heap);

    printf("heap after delete: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);

    return 0;
}
