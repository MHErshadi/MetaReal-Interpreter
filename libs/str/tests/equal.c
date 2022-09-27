/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 * 
 * Testing equal fuction
/*/

#include <str.h>
#include <stdio.h>

#define HEAP_SIZE 1024

#define CELLULAR_SIZE 1024
#define CELLULAR_UNIT sizeof(str_t)

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing equal function\n");

    heap_t heap;
    cellular_t cellular;

    heap_init(&heap, HEAP_SIZE);
    cellular_init(&cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    str_p str1 = str_set_str("Hello World", 11, &cellular, &heap);
    str_p str2 = str_set_str("Hello World", 11, &cellular, &heap);

    printf("str1 == str2: %d\n", str_equal(str1, str2, &cellular, &heap));

    str1 = str_set_str("Hello World", 11, &cellular, &heap);
    str2 = str_set_str("Hello Word!", 11, &cellular, &heap);

    printf("str1 == str2: %d\n", str_equal(str1, str2, &cellular, &heap));

    heap_delete(&heap);
    cellular_delete(&cellular);
    return 0;
}
