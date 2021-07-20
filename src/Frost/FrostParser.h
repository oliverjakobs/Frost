#ifndef FROST_PARSER_H
#define FROST_PARSER_H

#include "Frost.h"
#include "toolbox/tb_ini.h"

#define FROST_VARIANT_STR "<VARIANT>"

int FrostMatchVariant(char* ini, char* section, char* prop, int variant, int def);

/* ------------------| EntityState |-------------------------- */
int FrostParseEntityState(const char* str, size_t max_count);
const char* FrostEntityStateToString(EntityState value);

/* ------------------| Inventory |---------------------------- */
int FrostParseInventoryState(const char* str, size_t max_count);
int FrostParseInventoryAlignH(const char* str, size_t max_count);
int FrostParseInventoryAlignV(const char* str, size_t max_count);

/* ------------------| Interaction |-------------------------- */
int FrostParseInteraction(const char* str, size_t max_count);
int FrostParseInteractionType(const char* str, size_t max_count);

/* ------------------| RigidBody |---------------------------- */
int FrostParseRigidBodyType(const char* str, size_t max_count);
int FrostParseRigidBodyFilter(const char* str, size_t max_count);

/* ------------------| Input |-------------------------------- */
int FrostParseInputKeyCode(const char* str, size_t max_count);
int FrostParseInputMouseButton(const char* str, size_t max_count);

/* ------------------| Tile |--------------------------------- */
int FrostParseTileType(const char* str, size_t max_count);

#endif // !FROST_PARSER_H
