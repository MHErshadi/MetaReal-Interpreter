/*/
 * MetaReal version 1.0.0
 *
 * MetaReal Core verified as mr-core version 1.0.0
 * MetaReal Portal verified as mr-port version 1.0.0
/*/

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <interpreter/interpreter.h>
#include <setting.h>
#include <stdlib.h>
#include <string.h>
#include <info.h>
#include <def.h>

//#include <time.h>

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("MetaReal version %s\n", LANG_VER);
        printf("MetaReal Core verified as %s version %s\n", DEF_CORE, CORE_VER);
        printf("MetaReal Portal verified as %s version %s\n\n", DEF_PORT, PORT_VER);

        setting = setting_init();

        char* code = malloc(CMD_INPUT_SIZE);
        unsigned long long size;

        lres_t lres;
        pres_t pres;
        ires_t ires;

        table_t table = table_set(TABLE_SIZE);
        context_t context = context_set2(ROOT_CONTEXT, &table, CMD_FILE_NAME);

        while (1)
        {
            printf(">>> ");
            fgets(code, CMD_INPUT_SIZE, stdin);

            size = strlen(code);
            while (code[size - 2] == '\\')
            {
                code[size - 2] = '\n';

                printf("... ");
                fgets(code + size - 1, CMD_INPUT_SIZE - size, stdin);
                size = strlen(code);
            }
            code[size - 1] = '\0';

            for (; *code == ' ' || *code == '\t' || *code == '\n'; code++, size--);
            if (!*code)
                continue;

            lres = lex(code, '\0');
            if (lres.has_error)
            {
                illegal_char_print(&lres.error, code, size, CMD_FILE_NAME);
                continue;
            }

            pres = parse(lres.tokens);
            if (pres.has_error)
            {
                invalid_syntax_print(&pres.error, code, size, CMD_FILE_NAME);
                continue;
            }

            //clock_t s = clock();

            ires = interpret(pres.nodes, &context);
            if (ires.has_error)
            {
                runtime_print(&ires.error, code, size);
                continue;
            }

            //printf("time: %ld milliseconds\n", clock() - s);

            value_label(&ires.value, "\n");
            value_free(&ires.value);
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
