/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing set fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing set function\n");

    str_p str1 = str_set_str("Hello World", 11);
    str_p str2 = str_set(str1);

    printf("str2 = ");
    str_print(stdout, str2, "\n");

    str_free(str2);
    str_free(str1);
    return 0;
}
