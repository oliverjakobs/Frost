#include "tb_ini.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <stdio.h>

char* tb_ini_skip_whitespaces(char* cursor)
{
    while ((*cursor != '\0') && isspace(*cursor)) cursor++;
    return cursor;
}

char* tb_ini_parse_key(char* cursor, char* key)
{
    char* sp = cursor;
    while (*sp != '\0' && *sp != '=' && !isspace(*sp)) sp++;

    size_t key_len = sp - cursor;
    if (key_len >= TB_INI_NAME_LEN) key_len = TB_INI_NAME_LEN - 1;
    strncpy(key, cursor, key_len);
    key[key_len] = '\0';

    return tb_ini_skip_whitespaces(sp);
}

char* tb_ini_parse_section(char* cursor, tb_ini_section* section)
{
    char* sp = cursor;
    while (*sp != '\0' && *sp != ']') sp++;

    section->name_len = sp - cursor;
    if (section->name_len >= TB_INI_NAME_LEN) section->name_len = TB_INI_NAME_LEN - 1;

    strncpy(section->name, cursor, section->name_len);
    section->name[section->name_len] = '\0';

    printf("Section: \"%s\"\n", section->name);

    sp++; /* skip ']' after section name */
    sp = tb_ini_skip_whitespaces(sp);

    while (*sp != '\0' && *sp != '[')
    {
        /* read key */
        char key[TB_INI_NAME_LEN];
        sp = tb_ini_parse_key(sp, key);

        if (*sp != '=') break;

        printf("Key: \"%s\"\n", key);

        /* read value */
        sp = strchr(sp, '\n');

        sp = tb_ini_skip_whitespaces(sp);
    }

    /* skip till next */
    sp = strchr(sp, '[');

    return sp;
}

tb_ini_error tb_ini_parse(tb_ini* ini, char* data)
{
    ini->section_count = 3;
    ini->sections = malloc(ini->section_count * sizeof(tb_ini_section));

    if (!ini->sections) return TB_INI_MEM_ERROR;

    char* cursor = tb_ini_skip_whitespaces(data);

    for (size_t i = 0; i < 3; ++i)
    {
        if (*cursor++ == '[')
        {
            cursor = tb_ini_parse_section(cursor, ini->sections + i);
        }

    }

    return TB_INI_OK;
}

void tb_ini_destroy(tb_ini* ini)
{
    free(ini->sections);
    ini->section_count = 0;
}
