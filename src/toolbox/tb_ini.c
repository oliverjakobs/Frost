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

char* tb_ini_make_element(tb_ini_element* element, char* start, char* end)
{
    element->start = start;
    element->len = end - start;
    element->error = TB_INI_OK;
    return end;
}

char* tb_ini_make_error(tb_ini_element* element, tb_ini_error error, char* pos)
{
    element->start = pos;
    element->len = 0;
    element->error = error;
    return pos;
}

char* tb_ini_make_section(tb_ini_element* element, char* start)
{
    element->start = start;
    element->len = 0;
    while (*start != '\0' && *start != '[')
    {
        uint8_t not_blank = 0;
        while (*start != '\0' && *start != '\n')
        {
            if (!isspace(*start)) not_blank = 1;

            if (not_blank)  start = strchr(start, '\n');
            else            start++;
        }
        element->len += not_blank;
        if (*start != '\0') start++;
    }
    element->error = TB_INI_OK;
    return element->start;
}

char* tb_ini_read_value(char* ini, tb_ini_element* element)
{
    char* start = ini;
    if (*ini == '\"')
    {
        while (*ini != '\0' && *ini != '\n' && *ini != '\"') ini++;

        if (*ini++ != '\"') return tb_ini_make_error(element, TB_INI_UNTERMINATED_STR, start);

        return tb_ini_make_element(element, start, ini);
    }

    while (!isspace(*ini))
    {
        if (*ini == '\n' || *ini == '\0') return tb_ini_make_error(element, TB_INI_BAD_VALUE, ini);
        ini++;
    }

    return tb_ini_make_element(element, start, ini);
}

char* tb_ini_read_name(char* ini, tb_ini_element* element)
{
    if (!isalpha(*ini))
    {
        tb_ini_make_error(element, TB_INI_BAD_NAME, ini);
        return ini;
    }

    element->name = ini++;
    while (isalnum(*ini))
    {
        if (*ini == '\0') return tb_ini_make_error(element, TB_INI_BAD_NAME, ini);
        ini++;
    }
    element->name_len = ini - element->name;
    return ini;
}

char* tb_ini_read_element(char* ini, tb_ini_element* element)
{
    ini = tb_ini_skip_whitespaces(ini);

    ini = tb_ini_read_name(ini, element);
    if (element->error == TB_INI_BAD_NAME) return ini;

    ini = tb_ini_skip_whitespaces(ini);

    if (*ini++ == '=')
        return tb_ini_read_value(tb_ini_skip_whitespaces(ini), element);

    return tb_ini_make_error(element, TB_INI_BAD_PROPERTY, ini);
}

char* tb_ini_find_section(char* ini, char* name, size_t name_len, tb_ini_element* element)
{
    if (name_len == 0) return ini;
    while (*ini != '\0')
    {
        if (*ini != '[') ini++;
        else if (strncmp(++ini, name, name_len) == 0)
        {
            element->name = ini;
            element->name_len = name_len;
            while (*ini != '\0' && *ini != ']') ini++;
            return tb_ini_skip_whitespaces(++ini);
        }
    }

    return NULL;
}

char* tb_ini_query(char* ini, char* query, tb_ini_element* element)
{
    char* s_name = query;
    char* p_name = strchr(query, '.');

    size_t s_len = p_name ? p_name++ - s_name : strlen(query);

    char* section = tb_ini_find_section(ini, s_name, s_len, element);

    if (!section)   return tb_ini_make_error(element, TB_INI_BAD_SECTION, NULL);
    if (!p_name)    return tb_ini_make_section(element, section);

    return tb_ini_query_section(section, p_name, element);
}

char* tb_ini_query_section(char* section, char* query, tb_ini_element* element)
{
    size_t query_len = strlen(query);
    section = tb_ini_skip_whitespaces(section);

    if (query_len == 0)
        return tb_ini_read_element(section, element);

    while (*section != '\0' && *section != '[')
    {
        /* compare key */
        if (strncmp(section, query, query_len) == 0)
            return tb_ini_read_element(section, element);

        /* skip to next property */
        section = strchr(section, '\n');
        section = tb_ini_skip_whitespaces(section);
    }
    return NULL;
}

uint8_t tb_ini_query_bool(char* ini, char* query, uint8_t def)
{
    tb_ini_element element;
    tb_ini_query(ini, query, &element);

    if (element.error != TB_INI_OK) return def;
    return (strncmp(element.start, "true", element.len) == 0) ? 1 : 0;
}

int32_t tb_ini_query_int(char* ini, char* query, int32_t def)
{
    tb_ini_element element;
    tb_ini_query(ini, query, &element);

    return (element.error == TB_INI_OK) ? atoi(element.start) : def;
}

float tb_ini_query_float(char* ini, char* query, float def)
{
    tb_ini_element element;
    tb_ini_query(ini, query, &element);

    return (element.error == TB_INI_OK) ? (float)atof(element.start) : def;
}

size_t tb_ini_query_string(char* ini, char* query, char* dst, size_t dst_len)
{
    tb_ini_element element;
    tb_ini_query(ini, query, &element);

    return (element.error == TB_INI_OK) ? tb_ini_strncpy(dst, element.start, element.len, dst_len) : 0;
}

size_t tb_ini_name(const tb_ini_element* element, char* dst, size_t dst_len)
{
    return (element->error == TB_INI_OK) ? tb_ini_strncpy(dst, element->name, element->name_len, dst_len) : 0;
}

const char* tb_ini_get_error_desc(tb_ini_error error)
{
    switch (error)
    {
    case TB_INI_OK:                 return "no error";
    case TB_INI_BAD_NAME:           return "bad name";
    case TB_INI_BAD_VALUE:          return "bad value";
    case TB_INI_BAD_SECTION:        return "bad section";
    case TB_INI_BAD_PROPERTY:       return "bad property";
    case TB_INI_UNTERMINATED_STR:   return "unterminated string";
    default:                        return "unkown error";
    }
}