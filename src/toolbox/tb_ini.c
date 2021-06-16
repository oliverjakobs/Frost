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

/* TODO: make last section */
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

char* tb_ini_read_key(char* ini, tb_ini_element* element)
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

    ini = tb_ini_read_key(ini, element);
    if (element->error == TB_INI_BAD_NAME) return ini;

    ini = tb_ini_skip_whitespaces(ini);

    if (*ini++ == '=') return tb_ini_read_value(tb_ini_skip_whitespaces(ini), element);
    return tb_ini_make_error(element, TB_INI_BAD_PROPERTY, ini);
}

char* tb_ini_find_section(char* ini, const char* name, tb_ini_element* element)
{
    if (!name) return ini;

    size_t name_len = strlen(name);
    while (*ini != '\0')
    {
        if (*ini != '[') ini++;
        else if (strncmp(++ini, name, name_len) == 0)
        {
            char* start = ini;
            
            /* check if its the complete name */
            ini = tb_ini_skip_whitespaces(ini + name_len);

            if (*ini == ']')
            {
                element->name = start;
                element->name_len = name_len;
                return tb_ini_skip_whitespaces(++ini);
            }
        }
    }

    return NULL;
}

char* tb_ini_find_group(char* ini, const char* name, tb_ini_element* element)
{
    if (!name) return ini;

    size_t name_len = strlen(name);
    while (*ini != '\0')
    {
        if (*ini != '[') ini++;
        else if (strncmp(++ini, name, name_len) == 0)
        {
            ini += name_len;
            if (*ini == '.')
            {
                /* get section name */
                char* start = ++ini;
                while (*ini != ']')
                {
                    if (*ini == '\0') return NULL;
                    ini++;
                }
                element->name = start;
                element->name_len = ini - start;
                return tb_ini_skip_whitespaces(++ini);
            }
        }
    }

    return NULL;
}

char* tb_ini_query(char* ini, const char* section, const char* prop, tb_ini_element* element)
{
    char* section_start = tb_ini_find_section(ini, section, element);

    if (!section_start) return tb_ini_make_error(element, TB_INI_BAD_SECTION, NULL);
    if (!prop)          return tb_ini_make_section(element, section_start);

    return tb_ini_query_section(section_start, prop, element);
}

char* tb_ini_query_section(char* section, const char* prop, tb_ini_element* element)
{
    size_t query_len = strlen(prop);
    section = tb_ini_skip_whitespaces(section);

    if (query_len == 0) return tb_ini_read_element(section, element);

    while (*section != '\0' && *section != '[')
    {
        /* compare key */
        if (strncmp(section, prop, query_len) == 0)
            return tb_ini_read_element(section, element);

        /* skip to next property */
        section = strchr(section, '\n');
        section = tb_ini_skip_whitespaces(section);
    }
    return NULL;
}


char* tb_ini_query_group(char* ini, const char* group, tb_ini_element* element)
{
    if (!group) return NULL;
    char* start = tb_ini_find_group(ini, group, element);

    if (!start) return tb_ini_make_error(element, TB_INI_BAD_SECTION, NULL);
    return tb_ini_make_section(element, start);
}

/* utility */
uint8_t tb_ini_query_bool(char* ini, const char* section, const char* prop, uint8_t def)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

    if (element.error != TB_INI_OK) return def;
    return (strncmp(element.start, "true", element.len) == 0) ? 1 : 0;
}

int32_t tb_ini_query_int(char* ini, const char* section, const char* prop, int32_t def)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

    return (element.error == TB_INI_OK) ? atoi(element.start) : def;
}

float tb_ini_query_float(char* ini, const char* section, const char* prop, float def)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

    return (element.error == TB_INI_OK) ? (float)atof(element.start) : def;
}

size_t tb_ini_query_string(char* ini, const char* section, const char* prop, char* dst, size_t dst_len)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

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