/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Testing expand fuction
/*/

#include <memory.h>
#include <stdio.h>

#define HEAP_SIZE 1024

void free_block_print(free_block_p block);

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Heap form");
    puts("Testing expand function\n");

    heap_t heap;
    heap_init(&heap, HEAP_SIZE);

    printf("heap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    free_block_print(heap.free);

    void* ptr = heap_alloc(&heap, 20);
    heap_alloc(&heap, 15);

    printf("\nheap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    free_block_print(heap.free);

    heap_expand(&heap, ptr, 12);

    printf("\nheap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    free_block_print(heap.free);

    ptr = heap_alloc(&heap, 40);

    printf("\nheap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    free_block_print(heap.free);

    heap_expand(&heap, ptr, 99);

    printf("\nheap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    free_block_print(heap.free);

    heap_delete(&heap);
    return 0;
}

void free_block_print(free_block_p block)
{
    printf("free-block: {start=%p, next=%p, size=%llu}\n", block->start, block->next, block->size);

    if (block->next)
        free_block_print(block->next);
}
