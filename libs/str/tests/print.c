/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing print fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing print function\n");

    str_p str = str_set_str("Hello World", 11);
    str_print(stdout, str, "\n");

    str_free(str);
    return 0;
}
