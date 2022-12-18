/*/
 * MetaReal version 1.0.0
/*/

#include <def.h>
#include <stdlib.h>
#include <stdio.h>

void* m_alloc(unsigned long long size)
{
    return malloc(size);
}

void* m_realloc(void* block, unsigned long long size)
{
    return realloc(block, size);
}

void m_free(void* block)
{
    free(block);
}
