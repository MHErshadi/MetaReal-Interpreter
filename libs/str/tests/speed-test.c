/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
 *
 * Speed test
/*/

#include <str.h>
#include <stdio.h>
#include <time.h>

#define COUNT 100000000

int main()
{
    puts("String Library version 1.0.0");
    puts("Speed test\n");

    str_p str = str_set_str("Hello ", 6);

    clock_t start = clock();

    str_repeat(str, COUNT);

    clock_t end = clock();

    printf("Time: %ld milliseconds\n", end - start);

    str_free(str);
    return 0;
}
