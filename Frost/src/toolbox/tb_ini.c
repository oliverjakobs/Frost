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

void tb_ini_make_element(tb_ini_element* element, tb_ini_type type, char* start, size_t len)
{
    element->type = type;
    element->start = start;
    element->len = len;
    element->error = TB_INI_OK;
}

void tb_ini_make_error(tb_ini_element* element, tb_ini_error error, char* pos)
{
    element->type = TB_INI_ERROR;
    element->start = pos;
    element->len = 0;
    element->error = error;
}

size_t tb_ini_count_properties(char* section)
{
    size_t count = 0;
    while (*section != '\0' && *section != '[')
    {
        uint8_t not_blank = 0;
        while (*section != '\0' && *section != '\n')
        {
            if (!isspace(*section)) not_blank = 1;

            if (not_blank)  section = strchr(section, '\n');
            else            section++;
        }
        count += not_blank;
        if (*section != '\0') section++;
    }
    return count;
}

void tb_ini_make_section(tb_ini_element* element, char* start)
{
    element->type = TB_INI_SECTION;
    element->start = start;
    element->len = tb_ini_count_properties(start);
    element->error = TB_INI_OK;
}

char* tb_ini_read_element(char* ini, tb_ini_element* element)
{
    char* start = ini;
    switch (*ini++)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        while (*ini != '\0' && isdigit(*ini)) ini++;
        if (isspace(*ini))
            tb_ini_make_element(element, TB_INI_INT, start, ini - start);
        else
            tb_ini_make_error(element, TB_INI_BAD_NUMBER, start);
        break;
    case 't': case 'f':
        while (*ini != '\0' && !isspace(*ini)) ini++;
        if ((strncmp(start, "true", 4) == 0) || (strncmp(start, "false", 5) == 0))
            tb_ini_make_element(element, TB_INI_BOOL, start, ini - start);
        else
            tb_ini_make_error(element, TB_INI_UNKOWN_VALUE, start);
        break;
    case '\"':
        while (*ini != '\0' && *ini != '\n' && *ini != '\"') ini++;
        
        if (*ini++ == '\"')
            tb_ini_make_element(element, TB_INI_STRING, start, ini - start);
        else
            tb_ini_make_error(element, TB_INI_UNTERMINATED_STR, start);
        break;
    default:
        tb_ini_make_error(element, TB_INI_UNKOWN_ERROR, start);
        break;
    }

    return ini;
}

char* tb_ini_query_section(char* section, char* query, tb_ini_element* element)
{
    size_t query_len = strlen(query);
    section = tb_ini_skip_whitespaces(section);
    while (*section != '\0' && *section != '[')
    {
        /* compare key */
        if (strncmp(section, query, query_len) == 0)
        {
            section = tb_ini_skip_whitespaces(section + query_len);

            if (*section++ != '=')
                tb_ini_make_error(element, TB_INI_BAD_PROPERTY, section);

            /* read element */
            return tb_ini_read_element(tb_ini_skip_whitespaces(section), element);
        }

        /* skip to next property */
        section = strchr(section, '\n');
        section = tb_ini_skip_whitespaces(section);
    }
    return NULL;
}

char* tb_ini_find_section(char* ini, char* name, size_t name_len)
{
    if (name_len == 0) return ini;
    while (*ini != '\0')
    {
        if (*ini != '[') ini++;
        else if (strncmp(++ini, name, name_len) == 0)
        {
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

    char* section = tb_ini_find_section(ini, s_name, s_len);

    if (!section)
    {
        tb_ini_make_error(element, TB_INI_BAD_SECTION, ini);
        return NULL;
    }

    if (!p_name)
    {
        tb_ini_make_section(element, section);
        return section;
    }

    return tb_ini_query_section(section, p_name, element);
}

int32_t tb_ini_query_int(char* ini, char* query, int32_t def)
{
    tb_ini_element element;
    tb_ini_query(ini, query, &element);
    if (element.type != TB_INI_INT) return def;

    int32_t result;
    tb_ini_atoi(element.start, &result);
    return result;
}

uint8_t tb_ini_query_bool(char* ini, char* query, uint8_t def)
{
    tb_ini_element element;
    tb_ini_query(ini, query, &element);
    if (element.type != TB_INI_BOOL) return def;

    return *element.start == 't' ? 1 : 0;
}

size_t tb_ini_query_string(char* ini, char* query, char* dst, size_t dst_len)
{
    *dst = '\0';

    tb_ini_element element;
    tb_ini_query(ini, query, &element);
    if (element.type != TB_INI_STRING) return 0;

    return tb_ini_strncpy(dst, element.start + 1, element.len - 2, dst_len);
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

void tb_ini_print_element(tb_ini_element* element)
{
    if (element->type != TB_INI_ERROR)
        printf("%.*s (%s)", (uint32_t)element->len, element->start, tb_ini_get_type_name(element->type));
    else
        printf("ERROR: %s", tb_ini_get_error_desc(element->error));
}

const char* tb_ini_get_type_name(tb_ini_type type)
{
    switch (type)
    {
    case TB_INI_SECTION: return "section";
    case TB_INI_STRING:  return "string";
    case TB_INI_INT:     return "int";
    case TB_INI_BOOL:    return "bool";
    default:             return "error";
    }
}

const char* tb_ini_get_error_desc(tb_ini_error error)
{
    switch (error)
    {
    case TB_INI_OK:                 return "no error";
    case TB_INI_BAD_SECTION:        return "bad section";
    case TB_INI_BAD_PROPERTY:       return "bad property";
    case TB_INI_BAD_NUMBER:         return "bad number";
    case TB_INI_UNKOWN_VALUE:       return "unkown value type";
    case TB_INI_UNTERMINATED_STR:   return "unterminated string";
    case TB_INI_MEM_ERROR:          return "memory error";
    default:                        return "unkown error";
    }
}


/* WIP parsed structure approach */
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
    section->property_count = 0;

    char* start = stream;
    while (*stream != '\0' && *stream != ']') stream++;

    section->name_len = tb_ini_strncpy(section->name, start, stream - start, TB_INI_NAME_LEN);
    stream = tb_ini_skip_whitespaces(++stream); /* skip ']' and whitespaces after section name */

    while (*stream != '\0' && *stream != '[')
    {
        tb_ini_property property;

        /* read key */
        stream = tb_ini_parse_key(stream, &property);

        /* check and skip '=' */
        if (*stream++ != '=') break;

        /* read value */
        stream = tb_ini_parse_value(stream, &property);

        /* skip to next property */
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
    switch (property->type)
    {
    case TB_INI_STRING: printf("%s: \'%s\'\n", property->name, property->strval); break;
    case TB_INI_INT:    printf("%s: %d\n", property->name, property->ival); break;
    case TB_INI_BOOL:   printf("%s: %s\n", property->name, property->bval ? "true" : "false"); break;
    default:            printf("%s: %s\n", property->name, tb_ini_get_type_name(property->type)); break;
    }
}

void print_section(const tb_ini_section* section)
{
    printf("Section: <%s>\n", section->name);

    for (size_t i = 0; i < section->property_count; ++i)
        print_property(section->properties + i);
}

void print_ini(const tb_ini* ini)
{
    for (size_t i = 0; i < ini->section_count; ++i)
        print_section(ini->sections + i);
}