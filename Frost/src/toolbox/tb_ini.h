#ifndef TB_INI_H
#define TB_INI_H

#include <stddef.h>
#include <stdint.h>

#define TB_INI_NAME_LEN     32
#define TB_INI_VALUE_LEN    64

typedef enum
{
    TB_INI_OK,
    TB_INI_BAD_SECTION,
    TB_INI_BAD_PROPERTY,
    TB_INI_BAD_NUMBER,
    TB_INI_UNKOWN_VALUE,
    TB_INI_UNTERMINATED_STR,
    TB_INI_MEM_ERROR,
    TB_INI_UNKOWN_ERROR
} tb_ini_error;

typedef enum
{
    TB_INI_ERROR,
    TB_INI_SECTION,
    TB_INI_STRING,
    TB_INI_INT,
    TB_INI_BOOL
} tb_ini_type;

/* in place query functions */
typedef struct
{
    tb_ini_type type;
    char* start;
    size_t len;
    tb_ini_error error;
} tb_ini_element;

char* tb_ini_query(char* ini, char* query, tb_ini_element* element);
char* tb_ini_query_section(char* section, char* query, tb_ini_element* element);

int32_t tb_ini_query_int(char* ini, char* query, int32_t def);
uint8_t tb_ini_query_bool(char* ini, char* query, uint8_t def);
size_t tb_ini_query_string(char* ini, char* query, char* dst, size_t dst_len);

/*  */
char* tb_ini_atoi(char* p, int32_t* result);


void tb_ini_print_element(tb_ini_element* element);

const char* tb_ini_get_type_name(tb_ini_type type);
const char* tb_ini_get_error_desc(tb_ini_error error);


/* WIP parsed structure approach */
typedef struct
{
    char name[TB_INI_NAME_LEN];
    size_t name_len;

    tb_ini_type type;
    union
    {
        int32_t ival;
        uint8_t bval;
        struct
        {
            char strval[TB_INI_VALUE_LEN];
            size_t str_len;
        };
    };
} tb_ini_property;

typedef struct
{
    char name[TB_INI_NAME_LEN];
    size_t name_len;

    tb_ini_property* properties;
    size_t property_count;
} tb_ini_section;

typedef struct
{
    tb_ini_section* sections;
    size_t section_count;
} tb_ini;

tb_ini_error tb_ini_parse(tb_ini* ini, char* data);
void tb_ini_destroy(tb_ini* ini);

void print_property(const tb_ini_property* property);
void print_section(const tb_ini_section* section);
void print_ini(const tb_ini* ini);

#endif /* !TB_INI_H */
