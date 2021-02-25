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

size_t tb_ini_strncpy(char* dst, char* src, size_t len, size_t max_len)
{
    if (len >= max_len) len = max_len - 1;
    strncpy(dst, src, len);
    dst[len] = '\0';

    return len;
}

char* tb_ini_atoi(char* p, int32_t* result)
{
    uint8_t neg = 0;
    if (*p == '-')
    {
        neg = 1;
        ++p;
    }

    int32_t x = 0;
    while (*p >= '0' && *p <= '9')
    {
        x = (x * 10) + (*p - '0');
        ++p;
    }

    *result = neg ? -x : x;
    return p;
}

char* tb_ini_parse_key(char* stream, tb_ini_property* property)
{
    char* start = stream;
    while (*stream != '\0' && *stream != '=' && !isspace(*stream)) stream++;

    property->name_len = tb_ini_strncpy(property->name, start, stream - start, TB_INI_NAME_LEN);

    return tb_ini_skip_whitespaces(stream);
}

char* tb_ini_parse_value(char* stream, tb_ini_property* property)
{
    property->type = TB_INI_ERROR;

    stream = tb_ini_skip_whitespaces(stream);
    switch (*stream)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        stream = tb_ini_atoi(stream, &property->ival);
        if (isspace(*stream)) property->type = TB_INI_INT;
        break;
    case 't': case 'f':
    {
        char* start = stream;
        while (*stream != '\0' && !isspace(*stream)) stream++;

        if (strncmp(start, "true", stream - start) == 0) property->bval = 1;
        else if (strncmp(start, "false", stream - start) == 0) property->bval = 0;
        else break;
        property->type = TB_INI_BOOL;
        break;
    }
    case '\"':
    {
        char* start = ++stream; /* skip starting quote */
        while (*stream != '\"')
        {
            if (*stream == '\0' || *stream == '\n') return stream;
            stream++;
        }

        property->str_len = tb_ini_strncpy(property->strval, start, stream - start, TB_INI_VALUE_LEN);
        property->type = TB_INI_STRING;

        stream++; /* skip closing quote */
        break;
    }
    default:
        property->type = TB_INI_ERROR;
        break;
    }

    return stream;
}

char* tb_ini_parse_section(char* stream, tb_ini_section* section)
{
    char* start = stream;
    while (*stream != '\0' && *stream != ']') stream++;

    section->name_len = tb_ini_strncpy(section->name, start, stream - start, TB_INI_NAME_LEN);
    stream = tb_ini_skip_whitespaces(++stream); /* skip ']' and whitespaces after section name */

    printf("Section: <%s>\n", section->name);

    while (*stream != '\0' && *stream != '[')
    {
        tb_ini_property property;

        /* read key */
        stream = tb_ini_parse_key(stream, &property);

        /* check and skip '=' */
        if (*stream++ != '=') break;

        /* read value */
        stream = tb_ini_parse_value(stream, &property);

        print_property(&property);

        stream = tb_ini_skip_whitespaces(stream);
    }

    return stream;
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

void print_property(const tb_ini_property* property)
{
    if (property->type == TB_INI_STRING)
        printf("%s: \'%s\'\n", property->name, property->strval);
    else if (property->type == TB_INI_INT)
        printf("%s: %d\n", property->name, property->ival);
    else if (property->type == TB_INI_BOOL)
        printf("%s: %s\n", property->name, property->bval ? "true" : "false");
    else
        printf("%s: %s\n", property->name, tb_ini_get_type_name(property->type));
}

const char* tb_ini_get_type_name(tb_ini_value_type type)
{
    switch (type)
    {
    case TB_INI_STRING: return "string";
    case TB_INI_INT:    return "int";
    case TB_INI_BOOL:   return "bool";
    default:            return "error";
    }
}
