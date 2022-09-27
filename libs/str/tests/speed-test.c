/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Speed test
/*/

#include <str.h>
#include <stdio.h>
#include <time.h>

#define HEAP_SIZE 600000000

#define CELLULAR_SIZE 1024
#define CELLULAR_UNIT sizeof(str_t)

#define COUNT 100000000

int main()
{
    puts("String Library version 1.0.0");
    puts("Speed test\n");

    heap_t heap;
    cellular_t cellular;

    heap_init(&heap, HEAP_SIZE);
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    str_p str = str_set_str("Hello ", 6, &cellular, &heap);

    clock_t start = clock();

    str_repeat(str, COUNT, &heap);

    clock_t end = clock();

    printf("Time: %ld milliseconds\n", end - start);

    heap_delete(&heap);
    cellular_delete(&cellular);
    return 0;
}
