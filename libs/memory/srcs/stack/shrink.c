/*/
 * MetaReal version 1.0.0
 *
 * Memory Library version 1.0.0
 * Memory Stack form
 * 
 * Shrinks (block) down to (size) bytes
/*/

#include <memory.h>

void stack_shrink(stack_t stack, const void* block, unsigned long long size)
{
    stack->sp = (char*)block + size;
}
