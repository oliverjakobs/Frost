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
	size_t prefix_len = strlen(prefix);

	if (tb_strncasecmp(prefix, str, prefix_len) == 0)
		return InputParseMouseButton(str + prefix_len, max_count - prefix_len);

	/* named keys and function keys */
	switch (str[0])
	{
	case 'a': case 'A':
		if (tb_strncasecmp("apostrophe", str, max_count) == 0) return KEY_APOSTROPHE;
		break;
	case 'b': case 'B':
		if (tb_strncasecmp("backslash", str, max_count) == 0) return KEY_BACKSLASH;
		if (tb_strncasecmp("backspace", str, max_count) == 0) return KEY_BACKSPACE;
		break;
	case 'c': case 'C':
		if (tb_strncasecmp("comma", str, max_count) == 0) return KEY_COMMA;
		if (tb_strncasecmp("caps_lock", str, max_count) == 0) return KEY_CAPS_LOCK;
		break;
	case 'd': case 'D':
		if (tb_strncasecmp("delete", str, max_count) == 0) return KEY_DELETE;
		if (tb_strncasecmp("down", str, max_count) == 0) return KEY_DOWN;
		break;
	case 'e': case 'E':
		if (tb_strncasecmp("equal", str, max_count) == 0) return KEY_EQUAL;
		if (tb_strncasecmp("escape", str, max_count) == 0) return KEY_ESCAPE;
		if (tb_strncasecmp("enter", str, max_count) == 0) return KEY_ENTER;
		if (tb_strncasecmp("end", str, max_count) == 0) return KEY_END;
		break;
	case 'f': case 'F':
		if (tb_strncasecmp("f1", str, max_count) == 0) return KEY_F1;
		if (tb_strncasecmp("f2", str, max_count) == 0) return KEY_F2;
		if (tb_strncasecmp("f3", str, max_count) == 0) return KEY_F3;
		if (tb_strncasecmp("f4", str, max_count) == 0) return KEY_F4;
		if (tb_strncasecmp("f5", str, max_count) == 0) return KEY_F5;
		if (tb_strncasecmp("f6", str, max_count) == 0) return KEY_F6;
		if (tb_strncasecmp("f7", str, max_count) == 0) return KEY_F7;
		if (tb_strncasecmp("f8", str, max_count) == 0) return KEY_F8;
		if (tb_strncasecmp("f9", str, max_count) == 0) return KEY_F9;
		if (tb_strncasecmp("f10", str, max_count) == 0) return KEY_F10;
		if (tb_strncasecmp("f11", str, max_count) == 0) return KEY_F11;
		if (tb_strncasecmp("f12", str, max_count) == 0) return KEY_F12;
		if (tb_strncasecmp("f13", str, max_count) == 0) return KEY_F13;
		if (tb_strncasecmp("f14", str, max_count) == 0) return KEY_F14;
		if (tb_strncasecmp("f15", str, max_count) == 0) return KEY_F15;
		if (tb_strncasecmp("f16", str, max_count) == 0) return KEY_F16;
		if (tb_strncasecmp("f17", str, max_count) == 0) return KEY_F17;
		if (tb_strncasecmp("f18", str, max_count) == 0) return KEY_F18;
		if (tb_strncasecmp("f19", str, max_count) == 0) return KEY_F19;
		if (tb_strncasecmp("f20", str, max_count) == 0) return KEY_F20;
		if (tb_strncasecmp("f21", str, max_count) == 0) return KEY_F21;
		if (tb_strncasecmp("f22", str, max_count) == 0) return KEY_F22;
		if (tb_strncasecmp("f23", str, max_count) == 0) return KEY_F23;
		if (tb_strncasecmp("f24", str, max_count) == 0) return KEY_F24;
		if (tb_strncasecmp("f25", str, max_count) == 0) return KEY_F25;
		break;
	case 'g': case 'G':
		if (tb_strncasecmp("grave_accent", str, max_count) == 0) return KEY_GRAVE_ACCENT;
		break;
	case 'h': case 'H':
		if (tb_strncasecmp("home", str, max_count) == 0) return KEY_HOME;
		break;
	case 'i': case 'I':
		if (tb_strncasecmp("insert", str, max_count) == 0) return KEY_INSERT;
		break;
	case 'k': case 'K':
		if (tb_strncasecmp("kp_0", str, max_count) == 0) return KEY_KP_0;
		if (tb_strncasecmp("kp_1", str, max_count) == 0) return KEY_KP_1;
		if (tb_strncasecmp("kp_2", str, max_count) == 0) return KEY_KP_2;
		if (tb_strncasecmp("kp_3", str, max_count) == 0) return KEY_KP_3;
		if (tb_strncasecmp("kp_4", str, max_count) == 0) return KEY_KP_4;
		if (tb_strncasecmp("kp_5", str, max_count) == 0) return KEY_KP_5;
		if (tb_strncasecmp("kp_6", str, max_count) == 0) return KEY_KP_6;
		if (tb_strncasecmp("kp_7", str, max_count) == 0) return KEY_KP_7;
		if (tb_strncasecmp("kp_8", str, max_count) == 0) return KEY_KP_8;
		if (tb_strncasecmp("kp_9", str, max_count) == 0) return KEY_KP_9;
		if (tb_strncasecmp("kp_decimal", str, max_count) == 0) return KEY_KP_DECIMAL;
		if (tb_strncasecmp("kp_divide", str, max_count) == 0) return KEY_KP_DIVIDE;
		if (tb_strncasecmp("kp_multiply", str, max_count) == 0) return KEY_KP_MULTIPLY;
		if (tb_strncasecmp("kp_subtract", str, max_count) == 0) return KEY_KP_SUBTRACT;
		if (tb_strncasecmp("kp_add", str, max_count) == 0) return KEY_KP_ADD;
		if (tb_strncasecmp("kp_enter", str, max_count) == 0) return KEY_KP_ENTER;
		if (tb_strncasecmp("kp_equal", str, max_count) == 0) return KEY_KP_EQUAL;
		break;
	case 'l': case 'L':
		if (tb_strncasecmp("left", str, max_count) == 0) return KEY_LEFT;
		if (tb_strncasecmp("left_bracket", str, max_count) == 0) return KEY_LEFT_BRACKET;
		if (tb_strncasecmp("left_shift", str, max_count) == 0) return KEY_LEFT_SHIFT;
		if (tb_strncasecmp("left_control", str, max_count) == 0) return KEY_LEFT_CONTROL;
		if (tb_strncasecmp("left_alt", str, max_count) == 0) return KEY_LEFT_ALT;
		if (tb_strncasecmp("left_super", str, max_count) == 0) return KEY_LEFT_SUPER;
		break;
	case 'm': case 'M':
		if (tb_strncasecmp("minus", str, max_count) == 0) return KEY_MINUS;
		if (tb_strncasecmp("menu", str, max_count) == 0) return KEY_MENU;
		break;
	case 'n': case 'N':
		if (tb_strncasecmp("num_lock", str, max_count) == 0) return KEY_NUM_LOCK;
		break;
	case 'p': case 'P':
		if (tb_strncasecmp("period", str, max_count) == 0) return KEY_PERIOD;
		if (tb_strncasecmp("page_up", str, max_count) == 0) return KEY_PAGE_UP;
		if (tb_strncasecmp("page_down", str, max_count) == 0) return KEY_PAGE_DOWN;
		if (tb_strncasecmp("print_screen", str, max_count) == 0) return KEY_PRINT_SCREEN;
		if (tb_strncasecmp("pause", str, max_count) == 0) return KEY_PAUSE;
		break;
	case 'r': case 'R':
		if (tb_strncasecmp("right", str, max_count) == 0) return KEY_RIGHT;
		if (tb_strncasecmp("right_bracket", str, max_count) == 0) return KEY_RIGHT_BRACKET;
		if (tb_strncasecmp("right_shift", str, max_count) == 0) return KEY_RIGHT_SHIFT;
		if (tb_strncasecmp("right_control", str, max_count) == 0) return KEY_RIGHT_CONTROL;
		if (tb_strncasecmp("right_alt", str, max_count) == 0) return KEY_RIGHT_ALT;
		if (tb_strncasecmp("right_super", str, max_count) == 0) return KEY_RIGHT_SUPER;
		break;
	case 's': case 'S':
		if (tb_strncasecmp("space", str, max_count) == 0) return KEY_SPACE;
		if (tb_strncasecmp("slash", str, max_count) == 0) return KEY_SLASH;
		if (tb_strncasecmp("semicolon", str, max_count) == 0) return KEY_SEMICOLON;
		if (tb_strncasecmp("scroll_lock", str, max_count) == 0) return KEY_SCROLL_LOCK;
		break;
	case 't': case 'T':
		if (tb_strncasecmp("tab", str, max_count) == 0) return KEY_TAB;
		break;
	case 'u': case 'U':
		if (tb_strncasecmp("up", str, max_count) == 0) return KEY_UP;
		break;
	case 'w': case 'W':
		if (tb_strncasecmp("world_1", str, max_count) == 0) return KEY_WORLD_1;
		if (tb_strncasecmp("world_2", str, max_count) == 0) return KEY_WORLD_2;
		break;
	}

	return KEY_UNKNOWN;
}

int InputParseMouseButton(const char* str, size_t max_count)
{
	if (max_count == 0) return MOUSE_BUTTON_UNKNOWN;

	const char* prefix = "MOUSE_BUTTON_";
	size_t prefix_len = strlen(prefix);

	if (tb_strncasecmp(prefix, str, prefix_len) == 0)
		return InputParseMouseButton(str + prefix_len, max_count - prefix_len);

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
	/* named buttons */
	case 'l': case 'L':
		if (tb_strncasecmp("left", str, max_count) == 0) return MOUSE_BUTTON_LEFT;
		break;
	case 'r': case 'R':
		if (tb_strncasecmp("right", str, max_count) == 0) return MOUSE_BUTTON_RIGHT;
		break;
	case 'm': case 'M':
		if (tb_strncasecmp("middle", str, max_count) == 0) return MOUSE_BUTTON_MIDDLE;
		break;
	}
	return MOUSE_BUTTON_UNKNOWN;
}