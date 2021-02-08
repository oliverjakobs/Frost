#ifndef FROST_PARSER_H
#define FROST_PARSER_H

#include "Frost.h"
#include "toolbox/tb_json.h"

#define FROST_VARIANT_STR "<VARIANT>"

int FrostMatchVariant(char* json, char* query, int variant);

/* ------------------| EntityState |-------------------------- */
int FrostParseEntityState(const char* str, size_t max_count);
const char* FrostEntityStateToString(EntityState value);

/* ------------------| Inventory |---------------------------- */
int FrostParseInventoryState(const char* str, size_t max_count);
int FrostParseInventoryHAlign(const char* str, size_t max_count);
int FrostParseInventoryVAlign(const char* str, size_t max_count);

/* ------------------| Interaction |-------------------------- */
int FrostParseInteraction(const char* str, size_t max_count);
int FrostParseInteractionType(const char* str, size_t max_count);

/* ------------------| RigidBody |---------------------------- */
int FrostParseRigidBodyType(const char* str, size_t max_count);
int FrostParseRigidBodyFilter(const char* str, size_t max_count);

/* ------------------| Input |-------------------------------- */
int FrostParseInputKeyCode(const char* str, size_t max_count);
int FrostParseInputMouseButton(const char* str, size_t max_count);

#endif // !FROST_PARSER_H
