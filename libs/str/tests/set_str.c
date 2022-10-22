/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing set_str fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing set_str function\n");

    str_p str = str_set_str("Hello World", 11);

    printf("str = ");
    str_print(stdout, str, "\n");

    str_free(str);
    return 0;
}
