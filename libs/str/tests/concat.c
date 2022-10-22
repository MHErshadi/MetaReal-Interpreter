/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing concat fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing concat function\n");

    str_p str1 = str_set_str("Hello ", 6);
    str_p str2 = str_set_str("World", 5);

    str_concat(str1, str2);
    str_print(stdout, str1, "\n");

    str_free(str2);
    str_free(str1);
    return 0;
}
