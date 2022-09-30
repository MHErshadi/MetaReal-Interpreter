/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Speed test
/*/

#include <str.h>
#include <memory.h>
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

    heap_init(&memory.heap, HEAP_SIZE);
    cellular_init(&memory.str_cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    str_p str = str_set_str("Hello ", 6);

    clock_t start = clock();

    str_repeat(str, COUNT);

    clock_t end = clock();

    printf("Time: %ld milliseconds\n", end - start);

    heap_delete(&memory.heap);
    cellular_delete(&memory.str_cellular);
    return 0;
}
