/*/
 * MetaReal version 1.0.0
 *
 * MetaReal Core verified as mr-core version 1.0.0
 * MetaReal Portal verified as mr-port version 1.0.0
/*/

#include <debugger/errlib.h>
#include <setting.h>
#include <stdlib.h>
#include <string.h>
#include <info.h>
#include <def.h>

int main(int argc, char** argv)
{
    printf("MetaReal version %s\n", LANG_VER);
    printf("MetaReal Core verified as %s version %s\n", DEF_CORE, CORE_VER);
    printf("MetaReal Portal verified as %s version %s\n\n", DEF_PORT, PORT_VER);

    setting = setting_init();

    char* code = malloc(CMD_INPUT_SIZE);
    while (1)
    {
        printf(">>> ");
        fgets(code, CMD_INPUT_SIZE, stdin);

        illegal_char_t error = {*code, (pos_t){0, 1}};
        illegal_char_print(&error, code, strlen(code), CMD_FILE_NAME);
    }

    free(code);
    return 0;
}
