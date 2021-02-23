#ifndef TB_INI_H
#define TB_INI_H

#include <stddef.h>

typedef enum
{
    /* return types: */
    TB_INI_ERROR,       /* general error, eof etc. */
    TB_INI_PROPERTY,    
    TB_INI_SECTION,   
    TB_INI_STRING,      /* "string" */
    TB_INI_NUMBER,      /* number (may be -ve) int or float */
    TB_INI_BOOL,        /* true or false */
    TB_INI_NULL,        /* null */
    /* internal values: */
    TB_INI_ASSIGN,      /* "=" */
    TB_INI_EOL,         /* end of input string (ptr at '\0') */
} tb_ini_type;

typedef enum
{
    TB_JSON_OK,
} tb_ini_error;

typedef struct
{
    tb_ini_type type;   // type of the element
    size_t len;         // byte length of element
    char* value;        // pointer to value string in ini text
    tb_ini_error error; // error value if data_type == TB_INI_ERROR
} tb_ini_element;

char tb_ini_read(char* ini, tb_ini_element* result, char* query);

#endif /* !TB_INI_H */
