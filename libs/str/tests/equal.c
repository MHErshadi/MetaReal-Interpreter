/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing equal fuction
/*/

#include <str.h>
#include <memory.h>
#include <stdio.h>

#define HEAP_SIZE 1024

#define CELLULAR_SIZE 1024
#define CELLULAR_UNIT sizeof(str_t)

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing equal function\n");

    heap_init(&memory.heap, HEAP_SIZE);
    cellular_init(&memory.str_cellular, CELLULAR_SIZE, CELLULAR_UNIT);

    str_p str1 = str_set_str("Hello World", 11);
    str_p str2 = str_set_str("Hello World", 11);

    printf("str1 == str2: %d\n", str_equal(str1, str2));

    str_free(str2);
    str2 = str_set_str("Hello Word!", 11);

    printf("str1 == str2: %d\n", str_equal(str1, str2));

    heap_delete(&memory.heap);
    cellular_delete(&memory.str_cellular);
    return 0;
}
