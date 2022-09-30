/*/
 * MetaReal version 1.0.0
 *
 * String Library version 1.0.0
/*/

#ifndef __M_STR__
#define __M_STR__

#include <stdio.h>

struct __str__
{
    char* str;
    unsigned long long size;
};
typedef struct __str__ str_t;
typedef struct __str__* str_p;

str_p str_set(str_p src);
str_p str_set_str(const char* src, unsigned long long size);

void str_free(str_p str);

void str_print(FILE* stream, str_p str, const char* end);

/* */

void str_concat(str_p str1, str_p str2);
void str_remove(str_p str, unsigned long long pos);
void str_repeat(str_p str, unsigned long long count);

char str_equal(str_p str1, str_p str2);
char str_nequal(str_p str1, str_p str2);

char str_contains(str_p str, char chr);

#endif /* __M_STR__ */
