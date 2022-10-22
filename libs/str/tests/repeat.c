/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing repeat fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing repeat function\n");

    str_p str = str_set_str("Hello ", 6);

    str_repeat(str, 10);
    str_print(stdout, str, "\n");

    str_free(str);
    return 0;
}
