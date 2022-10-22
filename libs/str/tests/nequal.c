/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing nequal fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing nequal function\n");

    str_p str1 = str_set_str("Hello World", 11);
    str_p str2 = str_set_str("Hello World", 11);

    printf("str1 != str2: %d\n", str_equal(str1, str2));

    str_free(str2);
    str2 = str_set_str("Hello Word!", 11);

    printf("str1 != str2: %d\n", str_equal(str1, str2));

    str_free(str2);
    str_free(str1);
    return 0;
}
