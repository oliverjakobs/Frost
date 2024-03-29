#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "math/math.h"
#include "toolbox/tb_mem.h"

typedef enum
{
    TILE_LEFT,
    TILE_RIGHT,
    TILE_TOP,
    TILE_BOTTOM
} TileDirection;

typedef enum
{
    TILE_EMPTY = 0,
    TILE_SOLID,
    TILE_SLOPE_LEFT,
    TILE_SLOPE_RIGHT,
    TILE_PLATFORM,
    /* not implemented yet */
    TILE_CEIL_LEFT,
    TILE_CEIL_RIGHT
} TileType;

typedef enum
{
    TILE_REPLACE,		/* Sets tile at pos, overriding previous tiles */
    TILE_INSERT,		/* Sets tile at pos, if tile is currently empty (id == 0) */
    TILE_DELETE,		/* Deletes any tile at pos */
    TILE_DELETE_MATCH	/* Deletes tile at pos if it matches the specified tile */
} TileSetMode;

typedef uint32_t TileID;

typedef struct
{
    TileID id;
    vec2 pos;
    TileType type;
} Tile;

typedef struct
{
    size_t rows;
    size_t cols;

    float tile_size;

    Tile* tiles;

    int8_t borders[4];

    tb_allocator* allocator;

    /* types */
    TileType* types;
    size_t types_count;
} TileMap;

int TileMapLoad(TileMap* map, size_t rows, size_t cols, float tile_size, tb_allocator* allocator);
int TileMapLoadTiles(TileMap* map, TileID* tiles, TileType* types, size_t types_count);
void TileMapDestroy(TileMap* map);

int TileMapStreamTiles(TileMap* map, void* stream, void*(*next)(void*, TileID*), size_t len);
int TileMapStreamTypes(TileMap* map, void* stream, void*(*next)(void*, TileType*), size_t len);

size_t TileMapGetIndex(const TileMap* map, size_t row, size_t col);
const Tile* TileMapAt(const TileMap* map, size_t row, size_t col);
const Tile* TileMapAtPos(const TileMap* map, vec2 pos);

void TileMapSetAt(TileMap* map, size_t row, size_t col, TileID id, TileSetMode mode);
void TileMapSetAtPos(TileMap* map, vec2 pos, TileID id, TileSetMode mode);

int TileMapCheckType(const TileMap* map, vec2 pos, TileType type);
TileType TileMapGetType(const TileMap* map, TileID id);

int32_t TileMapClamp(const TileMap* map, float x);

void TileMapSetBorder(TileMap* map, TileDirection dir, int8_t value);
int8_t TileMapGetBorder(const TileMap* map, TileDirection dir);

#endif /* !TILE_MAP_H */
