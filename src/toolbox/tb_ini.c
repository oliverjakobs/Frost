#include "tb_ini.h"

#include <string.h>
#include <stdlib.h>

static inline int tb_ini_isendln(char c) { return c == '\n' || c == '\r' || c == '\0'; }
static inline int tb_ini_isblank(char c) { return c == ' ' || c == '\t' || c == '\v'; }
static inline int tb_ini_isspace(char c) { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v'; }
static inline int tb_ini_isalpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static inline int tb_ini_isalnum(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }

static char* tb_ini_skip(char* cursor, int(*what)(char))
{
    while (cursor && *cursor != '\0' && what(*cursor)) cursor++;
    return cursor;
}

/* removes trailing spaces ignoring current cursor pos */
static char* tb_ini_clip_tail(char* cursor)
{
    while (cursor && tb_ini_isblank(*(cursor - 1))) cursor--;
    return cursor;
}

static size_t tb_ini_strncpy(char* dst, char* src, size_t len, size_t max_len)
{
    if (len >= max_len) len = max_len - 1;
    strncpy(dst, src, len);
    dst[len] = '\0';

    return len;
}

static int tb_ini_string_quote(char* cursor, int multiline)
{
    if (multiline)  return (cursor[0] == '\"' && cursor[1] == '\"' && cursor[2] == '\"');
    else            return (cursor[0] == '\"');
}

/* create a value element with a length of (end-start) pointing to start */
static char* tb_ini_make_element(tb_ini_element* element, char* start, char* end)
{
    element->start = start;
    element->len = end - start;
    element->error = TB_INI_OK;
    return end;
}

/* create an error element pointing to pos */
static char* tb_ini_make_error(tb_ini_element* element, tb_ini_error error, char* pos)
{
    element->start = pos;
    element->len = 0;
    element->error = error;
    return pos;
}

static char* tb_ini_read_element(char* ini, tb_ini_element* element)
{
    /* read key */
    element->name = ini;

    while (ini && *ini != '\0' && *ini != '=' && !tb_ini_isendln(*ini)) ini++;

    element->name_len = tb_ini_clip_tail(ini) - element->name;

    /* check for '=' between key and value */
    ini = tb_ini_skip(ini, tb_ini_isspace);
    if (*ini != '=') return tb_ini_make_error(element, TB_INI_BAD_PROPERTY, ini);

    /* read the value*/
    ini = tb_ini_skip(++ini, tb_ini_isspace);

    char* start = ini;

    /* read grouped value */
    if (*ini == '{')
    {
        ini++; /* skip opening braces */

        while (*ini != '\0' && *ini != '}') ini++;

        if (*ini == '\0') return tb_ini_make_error(element, TB_INI_BAD_VALUE, start);

        ini++; /* skip closing braces */

        /* check if line is empty after grouped value */
        char* check = ini;
        while (!tb_ini_isendln(*check))
        {
            if (!tb_ini_isblank(*check)) return tb_ini_make_error(element, TB_INI_BAD_VALUE, check);
            check++;
        }

        return tb_ini_make_element(element, start, ini);
    }

    /* read standard value */
    while (ini && *ini != '\0' && !tb_ini_isendln(*ini)) ini++;

    return tb_ini_make_element(element, start, tb_ini_clip_tail(ini));
}

/* create a section element pointing to pos where len is the number of properties in that section */
static char* tb_ini_make_section(tb_ini_element* element, char* start)
{
    element->start = start;
    element->len = 0;
    element->error = TB_INI_OK;

    while (start && *start != '\0' && *start != '[')
    {
        tb_ini_element prop;
        start = tb_ini_read_element(start, &prop);

        if (prop.error != TB_INI_OK) return tb_ini_make_error(element, TB_INI_BAD_VALUE, prop.start);
        
        element->len++;
        start = tb_ini_skip(start, tb_ini_isspace);
    }
    return element->start;
}

static char* tb_ini_read_section(char* ini, const char* name, size_t len, tb_ini_element* element)
{
    char* start = ini;

    /* check if its the complete name */
    ini = tb_ini_skip(ini + len, tb_ini_isspace);

    if (*ini == ']')
    {
        element->name = start;
        element->name_len = len;
        return tb_ini_skip(++ini, tb_ini_isspace);
    }

    return ini;
}

static char* tb_ini_read_group(char* ini, const char* name, size_t len, tb_ini_element* element)
{
    ini += len;
    if (*ini == '.')
    {
        /* get section name */
        char* start = ++ini;
        while (*ini != ']' && *ini != '\0') ini++;

        if (*ini == '\0') return NULL;

        element->name = start;
        element->name_len = ini - start;

        return tb_ini_skip(++ini, tb_ini_isspace);
    }
    return ini;
}

static char* tb_ini_find_section(char* ini, const char* name, int group, tb_ini_element* element)
{
    if (!name) return ini;

    /* reset section element */
    element->name = NULL;
    element->name_len = 0;

    size_t name_len = strlen(name);
    while (*ini != '\0')
    {
        /* start of a new section found, check if name matches */
        if (*ini == '[' && strncmp(++ini, name, name_len) == 0)
        {
            /* read section or group depending on the flag set */
            if (group)  ini = tb_ini_read_group(ini, name, name_len, element);
            else        ini = tb_ini_read_section(ini, name, name_len, element);

            /* if ini == NULL: failed to read section/group -> return NULL */
            if (!ini) return NULL;

            /* if element->name_len > 0: successfully read section/group -> return cursor after it */
            if (element->name_len > 0) return ini;
        }
        ini++;
    }

    return NULL;
}

/* ----------------------------| Public API |------------------------------------------------------- */
char* tb_ini_query(char* ini, const char* section, const char* prop, tb_ini_element* element)
{
    char* section_start = tb_ini_find_section(ini, section, 0, element);

    if (!section_start) return tb_ini_make_error(element, TB_INI_BAD_SECTION, NULL);
    if (!prop)          return tb_ini_make_section(element, section_start);

    return tb_ini_query_section(section_start, prop, element);
}

char* tb_ini_query_section(char* section, const char* prop, tb_ini_element* element)
{
    size_t query_len = strlen(prop);
    section = tb_ini_skip(section, tb_ini_isspace);

    if (query_len == 0) return tb_ini_read_element(section, element);

    while (section && *section != '\0' && *section != '[')
    {
        /* compare key */
        if (strncmp(section, prop, query_len) == 0) return tb_ini_read_element(section, element);

        /* skip to next property */
        section = strpbrk(section, "\n\0");
        section = tb_ini_skip(section, tb_ini_isspace);
    }

    return tb_ini_make_error(element, TB_INI_BAD_PROPERTY, NULL);
}

char* tb_ini_query_group(char* ini, const char* group, tb_ini_element* element)
{
    if (!group) return NULL;
    char* start = tb_ini_find_section(ini, group, 1, element);

    if (!start) return tb_ini_make_error(element, TB_INI_BAD_SECTION, NULL);
    return tb_ini_make_section(element, start);
}

int tb_ini_query_bool(char* ini, const char* section, const char* prop, int def)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

    if (element.error != TB_INI_OK) return def;
    return (strncmp(element.start, "true", element.len) == 0) ? 1 : 0;
}

int tb_ini_query_int(char* ini, const char* section, const char* prop, int def)
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

int tb_ini_parse(char* ini, const char* section, const char* prop, tb_ini_parse_func parse)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

    return parse(element.start, element.len);
}

size_t tb_ini_name(const tb_ini_element* element, char* dst, size_t dst_len)
{
    return (element->error == TB_INI_OK) ? tb_ini_strncpy(dst, element->name, element->name_len, dst_len) : 0;
}

char* tb_ini_csv(char* ini, const char* section, const char* prop, tb_ini_element* element)
{
    tb_ini_query(ini, section, prop, element);
    if (element->error != TB_INI_OK) return element->start;

    /* check if element starts with a brace */
    if (!element->start || *element->start != '{') return tb_ini_make_error(element, TB_INI_BAD_VALUE, element->start);

    char* csv = ++element->start;
    element->len = 0;

    /* count values in csv list */
    while (*csv != '\0' && *csv != '}')
    {
        /* TODO: check for line end without comma (except for last value) */
        element->len += (*csv == ',');
        csv++;
    }

    /* add last element if csv is not empty */
    element->len += ((tb_ini_clip_tail(csv) - element->start) > 0);

    return csv;
}

char* tb_ini_csv_step(char* stream, tb_ini_element* element)
{
    if (!stream) return NULL;

    stream = tb_ini_skip(stream, tb_ini_isspace);
    element->start = stream;

    /* find end of value */
    while (!tb_ini_isendln(*stream) && *stream != '}' && *stream != ',') stream++;

    element->len = tb_ini_clip_tail(stream) - element->start;
    
    if (*stream == '}') return NULL; /* End of Stream */
    return ++stream; /* skip ',' after value */
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
    default:                        return "unkown error";
    }
}