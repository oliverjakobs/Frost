#include "FrostParser.h"

#include "toolbox/tb_str.h"

int InventoryParseState(const char* str, size_t max_count)
{
	if (max_count == 0) return INVENTORY_CLOSED;
	if (tb_strncasecmp(str, "open", max_count) == 0) return INVENTORY_OPEN;
	if (tb_strncasecmp(str, "fixed", max_count) == 0) return INVENTORY_FIXED;

	return INVENTORY_CLOSED;
}

int InventoryParseHAlign(const char* str, size_t max_count)
{
	if (max_count == 0) return INV_HALIGN_NONE;
	if (tb_strncasecmp(str, "left", max_count) == 0) return INV_HALIGN_LEFT;
	if (tb_strncasecmp(str, "center", max_count) == 0) return INV_HALIGN_CENTER;
	if (tb_strncasecmp(str, "right", max_count) == 0) return INV_HALIGN_RIGHT;

	return INV_HALIGN_NONE;
}

int InventoryParseVAlign(const char* str, size_t max_count)
{
	if (max_count == 0) return INV_VALIGN_NONE;
	if (tb_strncasecmp(str, "top", max_count) == 0) return INV_VALIGN_TOP;
	if (tb_strncasecmp(str, "center", max_count) == 0) return INV_VALIGN_CENTER;
	if (tb_strncasecmp(str, "bottom", max_count) == 0) return INV_VALIGN_BOTTOM;

	return INV_VALIGN_NONE;
}


int InteractionParse(const char* str, size_t max_count)
{
	if (strncmp(str, "TOGGLE_DOOR", max_count) == 0) return INTERACTION_TOGGLE_DOOR;
	if (strncmp(str, "OPEN_INVENTORY", max_count) == 0) return INTERACTION_OPEN_INVENTORY;

	return INTERACTION_NONE;
}

int InteractionParseType(const char* str, size_t max_count)
{
	if (strncmp(str, "TOGGLE", max_count) == 0) return INTERACTION_TYPE_TOGGLE;
	if (strncmp(str, "RANGED", max_count) == 0) return INTERACTION_TYPE_RANGED;
	if (strncmp(str, "TIMED", max_count) == 0) return INTERACTION_TYPE_TIMED;

	return INTERACTION_TYPE_TOGGLE;
}


int RigidBodyParseType(const char* str, size_t max_count)
{
	if (max_count == 0) return TILE_BODY_STATIC;
	if (tb_strncasecmp(str, "dynamic", max_count) == 0) return TILE_BODY_DYNAMIC;

	return TILE_BODY_STATIC;
}

int RigidBodyParseFilter(const char* str, size_t max_count)
{
	if (max_count == 0) return RIGID_BODY_FILTER_NONE;
	if (tb_strncasecmp(str, "world", max_count) == 0) return RIGID_BODY_FILTER_WORLD;
	if (tb_strncasecmp(str, "player", max_count) == 0) return RIGID_BODY_FILTER_PLAYER;
	if (tb_strncasecmp(str, "npc", max_count) == 0) return RIGID_BODY_FILTER_NPC;
	if (tb_strncasecmp(str, "door", max_count) == 0) return RIGID_BODY_FILTER_DOOR;

	return RIGID_BODY_FILTER_NONE;
}


static int InputParseKeyCodeChar(char c)
{
	switch (c)
	{
	case ' ': return KEY_SPACE;
	case '\'': return KEY_APOSTROPHE;
	case ',': return KEY_COMMA;
	case '-': return KEY_MINUS;
	case '.': return KEY_PERIOD;
	case '/': return KEY_SLASH;
	case ';': return KEY_SEMICOLON;
	case '=': return KEY_EQUAL;
	case '[': return KEY_LEFT_BRACKET;
	case '\\': return KEY_BACKSLASH;
	case ']': return KEY_RIGHT_BRACKET;
	case '`': return KEY_GRAVE_ACCENT;

	case '0': return KEY_0;
	case '1': return KEY_1;
	case '2': return KEY_2;
	case '3': return KEY_3;
	case '4': return KEY_4;
	case '5': return KEY_5;
	case '6': return KEY_6;
	case '7': return KEY_7;
	case '8': return KEY_8;
	case '9': return KEY_9;

	case 'a': case 'A': return KEY_A;
	case 'b': case 'B': return KEY_B;
	case 'c': case 'C': return KEY_C;
	case 'd': case 'D': return KEY_D;
	case 'e': case 'E': return KEY_E;
	case 'f': case 'F': return KEY_F;
	case 'g': case 'G': return KEY_G;
	case 'h': case 'H': return KEY_H;
	case 'i': case 'I': return KEY_I;
	case 'j': case 'J': return KEY_J;
	case 'k': case 'K': return KEY_K;
	case 'l': case 'L': return KEY_L;
	case 'm': case 'M': return KEY_M;
	case 'n': case 'N': return KEY_N;
	case 'o': case 'O': return KEY_O;
	case 'p': case 'P': return KEY_P;
	case 'q': case 'Q': return KEY_Q;
	case 'r': case 'R': return KEY_R;
	case 's': case 'S': return KEY_S;
	case 't': case 'T': return KEY_T;
	case 'u': case 'U': return KEY_U;
	case 'v': case 'V': return KEY_V;
	case 'w': case 'W': return KEY_W;
	case 'x': case 'X': return KEY_X;
	case 'y': case 'Y': return KEY_Y;
	case 'z': case 'Z': return KEY_Z;
	}

	return KEY_UNKNOWN;
}

int InputParseKeyCode(const char* str, size_t max_count)
{
	if (max_count == 0) return KEY_UNKNOWN;
	if (max_count == 1) return InputParseKeyCodeChar(str[0]);

	const char* prefix = "KEY_";

	return KEY_UNKNOWN;
}

int InputParseMouseButton(const char* str, size_t max_count)
{
	if (max_count == 0) return MOUSE_BUTTON_UNKNOWN;

	const char* prefix = "MOUSE_BUTTON_";

	switch (str[0])
	{
	case '1': return MOUSE_BUTTON_1;
	case '2': return MOUSE_BUTTON_2;
	case '3': return MOUSE_BUTTON_3;
	case '4': return MOUSE_BUTTON_4;
	case '5': return MOUSE_BUTTON_5;
	case '6': return MOUSE_BUTTON_6;
	case '7': return MOUSE_BUTTON_7;
	case '8': return MOUSE_BUTTON_8;

		/* TODO case insensitive compare */
	case 'l':
		if (strcmp(str, "left") == 0) return MOUSE_BUTTON_LEFT;
		break;
	case 'L':
		if (strcmp(str, "Left") == 0) return MOUSE_BUTTON_LEFT;
		if (strcmp(str, "LEFT") == 0) return MOUSE_BUTTON_LEFT;
		break;
	case 'r':
		if (strcmp(str, "right") == 0) return MOUSE_BUTTON_RIGHT;
		break;
	case 'R':
		if (strcmp(str, "Right") == 0) return MOUSE_BUTTON_RIGHT;
		if (strcmp(str, "RIGHT") == 0) return MOUSE_BUTTON_RIGHT;
		break;
	case 'm':
		if (strcmp(str, "middle") == 0) return MOUSE_BUTTON_MIDDLE;
		break;
	case 'M':
		if (strcmp(str, "Middle") == 0) return MOUSE_BUTTON_MIDDLE;
		if (strcmp(str, "MIDDLE") == 0) return MOUSE_BUTTON_MIDDLE;
		break;
	}
	return MOUSE_BUTTON_UNKNOWN;
}