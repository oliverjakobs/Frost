#include "tb_ini.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <stdio.h>

typedef enum
{
    TB_INI_ERROR,
    TB_INI_SECTION,
    TB_INI_EQUAL, 
    TB_INI_STRING,
    TB_INI_EOL,
} tb_ini_token;

char* tb_ini_skip_whitespaces(char* cursor)
{
    while ((*cursor != '\0') && isspace(*cursor)) cursor++;
    return cursor;
}

char* tb_ini_get_token(char* cursor, tb_ini_token* token)
{
    switch (*cursor++)
    {
    case '\0':  *token = TB_INI_EOL; break;
    case '"':   *token = TB_INI_STRING; break;
    case '[':   *token = TB_INI_SECTION; break;
    case '=':   *token = TB_INI_EQUAL; break;
    default:    *token = TB_INI_ERROR; break;
    }
    /*
    if (c == '\0') *token_type = TB_JSON_EOL;
    else if ((c == '"') || (c == TB_JSON_QUERY_QUOTE))  *token_type = TB_JSON_STRING;
    else if ((c >= '0') && (c <= '9'))                  *token_type = TB_JSON_NUMBER;
    else if ((c == 't') || (c == 'f'))                  *token_type = TB_JSON_BOOL;
    else if (c == '-') *token_type = TB_JSON_NUMBER;
    else if (c == '{') *token_type = TB_JSON_OBJECT;
    else if (c == '[') *token_type = TB_JSON_ARRAY;
    else if (c == '}') *token_type = TB_JSON_EOBJECT;
    else if (c == ']') *token_type = TB_JSON_EARRAY;
    else if (c == 'n') *token_type = TB_JSON_NULL;
    else if (c == ':') *token_type = TB_JSON_COLON;
    else if (c == ',') *token_type = TB_JSON_COMMA;
    else if (c == '*') *token_type = TB_JSON_QPARAM;
    else *token_type = TB_JSON_ERROR;
    */
    return cursor;
};

char* tb_ini_parse_key(char* cursor, char* key)
{
    cursor = tb_ini_skip_whitespaces(cursor);
    char* sp = cursor;

    while (*sp != '\0' && *sp != '=')
    {
        sp++;
    }

    size_t key_len = sp - cursor;
    if (key_len >= TB_INI_NAME_LEN) key_len = TB_INI_NAME_LEN - 1;
    strncpy(key, cursor, key_len);
    key[key_len] = '\0';

    return sp;
}

char* tb_ini_parse_section(char* cursor, tb_ini_section* section)
{
    char* sp = cursor;
    while (*sp != '\0' && *sp != ']') sp++;

    section->name_len = sp - cursor;
    if (section->name_len >= TB_INI_NAME_LEN) section->name_len = TB_INI_NAME_LEN - 1;

    strncpy(section->name, cursor, section->name_len);
    section->name[section->name_len] = '\0';

    printf("Section: %s\n", section->name);

    sp++; /* skip ']' after section name */

    /* read key */
    char key[TB_INI_NAME_LEN];
    sp = tb_ini_parse_key(sp, key);

    printf("Key: %s\n", key);

    /* skip till next */
    sp = strchr(sp, '[');

    return sp;
}

tb_ini_error tb_ini_parse(tb_ini* ini, const char* data)
{
    ini->section_count = 3;
    ini->sections = malloc(ini->section_count * sizeof(tb_ini_section));

    if (!ini->sections) return TB_INI_MEM_ERROR;

    char* cursor = (char*)data;

    tb_ini_token token;
    for (size_t i = 0; i < 3; ++i)
    {
        cursor = tb_ini_skip_whitespaces(cursor);
        cursor = tb_ini_get_token(cursor, &token);
        if (token == TB_INI_SECTION)
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
