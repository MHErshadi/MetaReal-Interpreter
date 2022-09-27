/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing repeat fuction
/*/

#include <str.h>
#include <stdio.h>

#define HEAP_SIZE 1024

#define CELLULAR_SIZE 1024
#define CELLULAR_UNIT sizeof(str_t)

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing repeat function\n");

    heap_t heap;
    cellular_t cellular;

    heap_init(&heap, HEAP_SIZE);
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    str_p str = str_set_str("Hello ", 6, &cellular, &heap);

    str_repeat(str, 10, &heap);
    str_print(stdout, str, "\n");

    heap_delete(&heap);
    cellular_delete(&cellular);
    return 0;
}
