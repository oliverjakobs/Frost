#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "Ignis/Ignis.h"

#include "toolbox/tb_hashmap.h"
#include "memory/arena.h"

typedef struct
{
	tb_hashmap fonts;	/* <str,IgnisFont> */
	Arena arena;
} FontManager;

int FontManagerInit(FontManager* manager, const char* path);
void FontManagerDestroy(FontManager* manager);

IgnisFont* FontManagerAddFont(FontManager* manager, const char* name, const char* path, float size);

IgnisFont* FontManagerGetFont(const FontManager* manager, const char* name);
const char* FontManagerGetFontName(const FontManager* manager, const IgnisFont* font);


#endif /* !FONT_MANAGER_H */