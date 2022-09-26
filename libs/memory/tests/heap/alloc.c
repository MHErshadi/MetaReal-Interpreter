/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Heap form
 * 
 * Testing alloc fuction
/*/

#include <memory.h>
#include <stdio.h>
#include <string.h>

#define HEAP_SIZE 1024

struct vector
{
    int x;
    int y;
    int z;
};

int main()
{
    puts("Memory Library version 1.0.0");
    puts("Memory Heap form");
    puts("Testing init function\n");

    heap_t heap;
    heap_init(&heap, HEAP_SIZE);

    printf("heap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block: {start=%p, next=%p, size=%llu}\n\n", heap.free->start, heap.free->next, heap.free->size);

    char* str = heap_alloc(&heap, 12);
    strncpy(str, "Hello World", 12);

    printf("heap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block: {start=%p, next=%p, size=%llu}\n\n", heap.free->start, heap.free->next, heap.free->size);

    void* ptr = heap_alloc(&heap, sizeof(int));

    printf("heap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block: {start=%p, next=%p, size=%llu}\n\n", heap.free->start, heap.free->next, heap.free->size);

    struct vector* vec = heap_alloc(&heap, sizeof(struct vector));
    *vec = (struct vector){19, 38, -3};

    printf("heap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block: {start=%p, next=%p, size=%llu}\n\n", heap.free->start, heap.free->next, heap.free->size);

    heap_free(&heap, ptr, sizeof(int));

    int* age = heap_alloc(&heap, sizeof(int));
    *age = 100;

    printf("heap: {data=%p, size=%llu, end=%p, free=%p, temp=%p}\n", heap.data, heap.size, heap.end, heap.free, heap.temp);
    printf("free-block: {start=%p, next=%p, size=%llu}\n\n", heap.free->start, heap.free->next, heap.free->size);

    printf("heap data character form:\n");
    unsigned long long i;
    for (i = 0; i < HEAP_SIZE; i++)
        printf("%c", heap.data[i]);
    printf("\n\n");

    printf("heap data int form:\n");
    for (i = 0; i < HEAP_SIZE; i++)
        printf("%d ", heap.data[i]);
    printf("\n");

    heap_delete(&heap);
    return 0;
}
