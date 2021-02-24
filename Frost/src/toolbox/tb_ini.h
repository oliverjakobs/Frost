#ifndef TB_INI_H
#define TB_INI_H

#include <stddef.h>
#include <stdint.h>

#define TB_INI_NAME_LEN     32
#define TB_INI_VALUE_LEN    64

typedef enum
{
    TB_INI_OK,
    TB_INI_MEM_ERROR
} tb_ini_error;

typedef enum
{
    TB_INI_ERROR,
    TB_INI_STRING,
    TB_INI_INT,
    TB_INI_BOOL
} tb_ini_value_type;

typedef struct
{
    char name[TB_INI_NAME_LEN];
    size_t name_len;

    tb_ini_value_type type;
    union
    {
        int32_t ival;
        uint8_t bval;
        char strval[TB_INI_VALUE_LEN];
    };

} tb_ini_property;

typedef struct
{
    char name[TB_INI_NAME_LEN];
    size_t name_len;
} tb_ini_section;

typedef struct
{
    tb_ini_section* sections;
    size_t section_count;
} tb_ini;

tb_ini_error tb_ini_parse(tb_ini* ini, char* data);
void tb_ini_destroy(tb_ini* ini);

#endif /* !TB_INI_H */
