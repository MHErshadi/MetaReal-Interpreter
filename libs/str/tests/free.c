/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Testing free fuction
/*/

#include <str.h>
#include <stdio.h>

int main()
{
    puts("String Library version 1.0.0");
    puts("Testing free function\n");

    str_p str = str_set_str("Hello World", 11);

    printf("str before free: {str=%p, size=%llu}\n\n", str->str, str->size);

    str_free(str);

    printf("str after free: {str=%p, size=%llu}\n", str->str, str->size);

    str_free(str);
    return 0;
}
