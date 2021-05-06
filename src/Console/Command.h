#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

/* 
 * Check the rest of a potential cmd�s lexeme 
 * Returns cmd if it is a match or 0 if not 
 */
uint32_t cmd_check_keyword(const char* buffer, const char* rest, uint32_t cmd);

char* cmd_get_args(const char* buffer, int offset, char** args, int arg_count);

#endif /* COMMAND_H */