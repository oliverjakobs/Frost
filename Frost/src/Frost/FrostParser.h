#ifndef FROST_PARSER_H
#define FROST_PARSER_H

#include "Frost.h"

int InventoryParseState(const char* str, size_t max_count);
int InventoryParseHAlign(const char* str, size_t max_count);
int InventoryParseVAlign(const char* str, size_t max_count);


int InteractionParse(const char* str, size_t max_count);
int InteractionParseType(const char* str, size_t max_count);


int RigidBodyParseType(const char* str, size_t max_count);
int RigidBodyParseFilter(const char* str, size_t max_count);


int InputParseKeyCode(const char* str, size_t max_count);
int InputParseMouseButton(const char* str, size_t max_count);

#endif // !FROST_PARSER_H
