#ifndef TB_INI_H
#define TB_INI_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    TB_INI_OK,
    TB_INI_BAD_NAME,
    TB_INI_BAD_VALUE,
    TB_INI_BAD_SECTION,
    TB_INI_BAD_PROPERTY,
    TB_INI_UNKOWN_ERROR
} tb_ini_error;

/* in place query functions */
typedef struct
{
    char* name;
    size_t name_len;    /* bytelen of name */
    char* start;
    size_t len;         /* bytelen of value_str or num of properites in section if type == TB_INI_SECTION */
    tb_ini_error error;
} tb_ini_element;

char* tb_ini_query(char* ini, const char* section, const char* prop, tb_ini_element* element);

/* searches for the given property in the current section (until '[' is reached) */
char* tb_ini_query_section(char* section, const char* prop, tb_ini_element* element);

/* returns the next section in the group */
char* tb_ini_query_group(char* ini, const char* group, tb_ini_element* element);

uint8_t tb_ini_query_bool(char* ini, const char* section, const char* prop, uint8_t def);
int32_t tb_ini_query_int(char* ini, const char* section, const char* prop, int32_t def);
float tb_ini_query_float(char* ini, const char* section, const char* prop, float def);
size_t tb_ini_query_string(char* ini, const char* section, const char* prop, char* dst, size_t dst_len);

size_t tb_ini_name(const tb_ini_element* element, char* dst, size_t dst_len);

const char* tb_ini_get_error_desc(tb_ini_error error);

#endif /* !TB_INI_H */
