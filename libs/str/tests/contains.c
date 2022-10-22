/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing contains fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing contains function\n");

    str_p str = str_set_str("Hello World", 11);

    printf("str contains 'e': %d\n", str_contains(str, 'e'));
    printf("str contains 'p': %d\n", str_contains(str, 'p'));

    str_free(str);
    return 0;
}
