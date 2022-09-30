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
    if (argc == 1)
    {
        printf("MetaReal version %s\n", LANG_VER);
        printf("MetaReal Core verified as %s version %s\n", DEF_CORE, CORE_VER);
        printf("MetaReal Portal verified as %s version %s\n\n", DEF_PORT, PORT_VER);

        setting = setting_init();

        char* code = malloc(CMD_INPUT_SIZE);
        while (1)
        {
            printf(">>> ");
            gets_s(code, CMD_INPUT_SIZE);

            illegal_char_t error = {*code, (pos_t){0, 1}};
            illegal_char_print(&error, code, strlen(code), CMD_FILE_NAME);
        }

        free(code);
        return 0;
    }

    char* first = argv[1];
    if (*first == '-')
    {
        if (!strcmp(first, "--help"))
        {
            puts("usage: metareal [options] [file] [commands]\n");
            puts("commands:");
            puts("\t--version: prints version of MetaReal, Core and Portal");
            puts("\t--dump-ver: dumps version of MetaReal");
            puts("\t--dump-core: dumps registered Core");
            puts("\t--dump-core-ver: dumps version of Core");
            puts("\t--dump-port: dumps registered Portal");
            puts("\t--dump-port-ver: dumps version of Portal");
            return 0;
        }
        if (!strcmp(first, "--version"))
        {
            printf("MetaReal version %s\n", LANG_VER);
            printf("MetaReal Core verified as %s version %s\n", DEF_CORE, CORE_VER);
            printf("MetaReal Portal verified as %s version %s\n", DEF_PORT, PORT_VER);
            return 0;
        }
        if (!strcmp(first, "--dump-ver"))
        {
            puts(LANG_VER);
            return 0;
        }
        if (!strcmp(first, "--dump-core"))
        {
            puts(DEF_CORE);
            return 0;
        }
        if (!strcmp(first, "--dump-core-ver"))
        {
            puts(CORE_VER);
            return 0;
        }
        if (!strcmp(first, "--dump-port"))
        {
            puts(DEF_PORT);
            return 0;
        }
        if (!strcmp(first, "--dump-port-ver"))
        {
            puts(PORT_VER);
            return 0;
        }
    }

    fprintf(stderr, "undefined command \"%s\", see --help for more information\n", first);
    return 0;
}
