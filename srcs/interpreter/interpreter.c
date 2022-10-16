/*/
 * MetaReal version 1.0.0
/*/

#include <interpreter/interpreter.h>
#include <stdlib.h>

ires_t interpret(node_p nodes, context_p context)
{
    ires_t ires;
    ires.has_error = 0;

    free(nodes);

    return ires;
}
