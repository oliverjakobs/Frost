#include "FrostParser.h"

#include "toolbox/tb_str.h"

int FrostMatchVariant(char* ini, char* section, char* prop, int variant, int def)
{
    tb_ini_element element;
    tb_ini_query(ini, section, prop, &element);

    if (element.error != TB_INI_OK) return def;
    if (strncmp(element.start, FROST_VARIANT_STR, element.len) == 0) return variant;

    return tb_ini_element_to_int(&element);
}

/* ------------------| EntityState |-------------------------- */
int FrostParseEntityState(const char* str, size_t max_count)
{
    if (max_count == 0) return ENTITY_STATE_NULL;
    if (tb_strncasecmp("idle", str, max_count) == 0) return ENTITY_STATE_IDLE;
    if (tb_strncasecmp("walk", str, max_count) == 0) return ENTITY_STATE_WALK;
    if (tb_strncasecmp("jump", str, max_count) == 0) return ENTITY_STATE_JUMP;
    if (tb_strncasecmp("fall", str, max_count) == 0) return ENTITY_STATE_FALL;

    return ENTITY_STATE_NULL;
}

const char* FrostEntityStateToString(EntityState value)
{
    switch (value)
    {
    case ENTITY_STATE_IDLE: return "idle";
    case ENTITY_STATE_WALK: return "walk";
    case ENTITY_STATE_JUMP: return "jump";
    case ENTITY_STATE_FALL: return "fall";
    default: return "null";
    }
}

/* ------------------| Inventory |---------------------------- */
int FrostParseInventoryState(const char* str, size_t max_count)
{
    if (max_count == 0) return INVENTORY_CLOSED;
    if (tb_strncasecmp("open", str, max_count) == 0)    return INVENTORY_OPEN;
    if (tb_strncasecmp("fixed", str, max_count) == 0)   return INVENTORY_FIXED;

    return INVENTORY_CLOSED;
}

int FrostParseInventoryAlignH(const char* str, size_t max_count)
{
    if (max_count == 0) return INV_ALIGN_H_NONE;
    if (tb_strncasecmp("left", str, max_count) == 0)    return INV_ALIGN_H_LEFT;
    if (tb_strncasecmp("center", str, max_count) == 0)  return INV_ALIGN_H_CENTER;
    if (tb_strncasecmp("right", str, max_count) == 0)   return INV_ALIGN_H_RIGHT;

    return INV_ALIGN_H_NONE;
}

int FrostParseInventoryAlignV(const char* str, size_t max_count)
{
    if (max_count == 0) return INV_ALIGN_V_NONE;
    if (tb_strncasecmp("top", str, max_count) == 0)     return INV_ALIGN_V_TOP;
    if (tb_strncasecmp("center", str, max_count) == 0)  return INV_ALIGN_V_CENTER;
    if (tb_strncasecmp("bottom", str, max_count) == 0)  return INV_ALIGN_V_BOTTOM;

    return INV_ALIGN_V_NONE;
}

/* ------------------| Interaction |-------------------------- */
int FrostParseInteraction(const char* str, size_t max_count)
{
    if (max_count == 0) return INTERACTION_NONE;
    if (tb_strncasecmp("toggle_door", str, max_count) == 0) return INTERACTION_TOGGLE_DOOR;
    if (tb_strncasecmp("open_inventory", str, max_count) == 0) return INTERACTION_OPEN_INVENTORY;

    return INTERACTION_NONE;
}

int FrostParseInteractionType(const char* str, size_t max_count)
{
    if (max_count == 0) return INTERACTION_TYPE_TOGGLE;
    if (tb_strncasecmp("toggle", str, max_count) == 0) return INTERACTION_TYPE_TOGGLE;
    if (tb_strncasecmp("ranged", str, max_count) == 0) return INTERACTION_TYPE_RANGED;
    if (tb_strncasecmp("timed", str, max_count) == 0) return INTERACTION_TYPE_TIMED;

    return INTERACTION_TYPE_TOGGLE;
}

/* ------------------| RigidBody |---------------------------- */
int FrostParseRigidBodyType(const char* str, size_t max_count)
{
    if (max_count == 0) return TILE_BODY_STATIC;
    if (tb_strncasecmp("dynamic", str, max_count) == 0) return TILE_BODY_DYNAMIC;

    return TILE_BODY_STATIC;
}

int FrostParseRigidBodyFilter(const char* str, size_t max_count)
{
    if (max_count == 0) return RIGID_BODY_FILTER_NONE;
    if (tb_strncasecmp("world", str, max_count) == 0) return RIGID_BODY_FILTER_WORLD;
    if (tb_strncasecmp("player", str, max_count) == 0) return RIGID_BODY_FILTER_PLAYER;
    if (tb_strncasecmp("npc", str, max_count) == 0) return RIGID_BODY_FILTER_NPC;
    if (tb_strncasecmp("door", str, max_count) == 0) return RIGID_BODY_FILTER_DOOR;

    return RIGID_BODY_FILTER_NONE;
}

/* ------------------| Input |-------------------------------- */
static int FrostParseInputKeyCodeChar(char c)
{
    switch (c)
    {
    case ' ':  return GLFW_KEY_SPACE;
    case '\'': return GLFW_KEY_APOSTROPHE;
    case ',':  return GLFW_KEY_COMMA;
    case '-':  return GLFW_KEY_MINUS;
    case '.':  return GLFW_KEY_PERIOD;
    case '/':  return GLFW_KEY_SLASH;
    case ';':  return GLFW_KEY_SEMICOLON;
    case '=':  return GLFW_KEY_EQUAL;
    case '[':  return GLFW_KEY_LEFT_BRACKET;
    case '\\': return GLFW_KEY_BACKSLASH;
    case ']':  return GLFW_KEY_RIGHT_BRACKET;
    case '`':  return GLFW_KEY_GRAVE_ACCENT;

    case '0': return GLFW_KEY_0;
    case '1': return GLFW_KEY_1;
    case '2': return GLFW_KEY_2;
    case '3': return GLFW_KEY_3;
    case '4': return GLFW_KEY_4;
    case '5': return GLFW_KEY_5;
    case '6': return GLFW_KEY_6;
    case '7': return GLFW_KEY_7;
    case '8': return GLFW_KEY_8;
    case '9': return GLFW_KEY_9;

    case 'a': case 'A': return GLFW_KEY_A;
    case 'b': case 'B': return GLFW_KEY_B;
    case 'c': case 'C': return GLFW_KEY_C;
    case 'd': case 'D': return GLFW_KEY_D;
    case 'e': case 'E': return GLFW_KEY_E;
    case 'f': case 'F': return GLFW_KEY_F;
    case 'g': case 'G': return GLFW_KEY_G;
    case 'h': case 'H': return GLFW_KEY_H;
    case 'i': case 'I': return GLFW_KEY_I;
    case 'j': case 'J': return GLFW_KEY_J;
    case 'k': case 'K': return GLFW_KEY_K;
    case 'l': case 'L': return GLFW_KEY_L;
    case 'm': case 'M': return GLFW_KEY_M;
    case 'n': case 'N': return GLFW_KEY_N;
    case 'o': case 'O': return GLFW_KEY_O;
    case 'p': case 'P': return GLFW_KEY_P;
    case 'q': case 'Q': return GLFW_KEY_Q;
    case 'r': case 'R': return GLFW_KEY_R;
    case 's': case 'S': return GLFW_KEY_S;
    case 't': case 'T': return GLFW_KEY_T;
    case 'u': case 'U': return GLFW_KEY_U;
    case 'v': case 'V': return GLFW_KEY_V;
    case 'w': case 'W': return GLFW_KEY_W;
    case 'x': case 'X': return GLFW_KEY_X;
    case 'y': case 'Y': return GLFW_KEY_Y;
    case 'z': case 'Z': return GLFW_KEY_Z;
    }

    return GLFW_KEY_UNKNOWN;
}

int FrostParseInputKeyCode(const char* str, size_t max_count)
{
    if (max_count == 0) return GLFW_KEY_UNKNOWN;
    if (max_count == 1) return FrostParseInputKeyCodeChar(str[0]);

    const char* prefix = "KEY_";
    size_t prefix_len = strlen(prefix);

    if (tb_strncasecmp(prefix, str, prefix_len) == 0)
        return FrostParseInputKeyCode(str + prefix_len, max_count - prefix_len);

    /* named keys and function keys */
    switch (str[0])
    {
    case 'a': case 'A':
        if (tb_strncasecmp("apostrophe", str, max_count) == 0) return GLFW_KEY_APOSTROPHE;
        break;
    case 'b': case 'B':
        if (tb_strncasecmp("backslash", str, max_count) == 0) return GLFW_KEY_BACKSLASH;
        if (tb_strncasecmp("backspace", str, max_count) == 0) return GLFW_KEY_BACKSPACE;
        break;
    case 'c': case 'C':
        if (tb_strncasecmp("comma", str, max_count) == 0) return GLFW_KEY_COMMA;
        if (tb_strncasecmp("caps_lock", str, max_count) == 0) return GLFW_KEY_CAPS_LOCK;
        break;
    case 'd': case 'D':
        if (tb_strncasecmp("delete", str, max_count) == 0) return GLFW_KEY_DELETE;
        if (tb_strncasecmp("down", str, max_count) == 0) return GLFW_KEY_DOWN;
        break;
    case 'e': case 'E':
        if (tb_strncasecmp("equal", str, max_count) == 0) return GLFW_KEY_EQUAL;
        if (tb_strncasecmp("escape", str, max_count) == 0) return GLFW_KEY_ESCAPE;
        if (tb_strncasecmp("enter", str, max_count) == 0) return GLFW_KEY_ENTER;
        if (tb_strncasecmp("end", str, max_count) == 0) return GLFW_KEY_END;
        break;
    case 'f': case 'F':
        if (tb_strncasecmp("f1", str, max_count) == 0)  return GLFW_KEY_F1;
        if (tb_strncasecmp("f2", str, max_count) == 0)  return GLFW_KEY_F2;
        if (tb_strncasecmp("f3", str, max_count) == 0)  return GLFW_KEY_F3;
        if (tb_strncasecmp("f4", str, max_count) == 0)  return GLFW_KEY_F4;
        if (tb_strncasecmp("f5", str, max_count) == 0)  return GLFW_KEY_F5;
        if (tb_strncasecmp("f6", str, max_count) == 0)  return GLFW_KEY_F6;
        if (tb_strncasecmp("f7", str, max_count) == 0)  return GLFW_KEY_F7;
        if (tb_strncasecmp("f8", str, max_count) == 0)  return GLFW_KEY_F8;
        if (tb_strncasecmp("f9", str, max_count) == 0)  return GLFW_KEY_F9;
        if (tb_strncasecmp("f10", str, max_count) == 0) return GLFW_KEY_F10;
        if (tb_strncasecmp("f11", str, max_count) == 0) return GLFW_KEY_F11;
        if (tb_strncasecmp("f12", str, max_count) == 0) return GLFW_KEY_F12;
        break;
    case 'g': case 'G':
        if (tb_strncasecmp("grave_accent", str, max_count) == 0) return GLFW_KEY_GRAVE_ACCENT;
        break;
    case 'h': case 'H':
        if (tb_strncasecmp("home", str, max_count) == 0) return GLFW_KEY_HOME;
        break;
    case 'i': case 'I':
        if (tb_strncasecmp("insert", str, max_count) == 0) return GLFW_KEY_INSERT;
        break;
    case 'k': case 'K':
        if (tb_strncasecmp("kp_0", str, max_count) == 0)		return GLFW_KEY_KP_0;
        if (tb_strncasecmp("kp_1", str, max_count) == 0)		return GLFW_KEY_KP_1;
        if (tb_strncasecmp("kp_2", str, max_count) == 0)		return GLFW_KEY_KP_2;
        if (tb_strncasecmp("kp_3", str, max_count) == 0)		return GLFW_KEY_KP_3;
        if (tb_strncasecmp("kp_4", str, max_count) == 0)		return GLFW_KEY_KP_4;
        if (tb_strncasecmp("kp_5", str, max_count) == 0)		return GLFW_KEY_KP_5;
        if (tb_strncasecmp("kp_6", str, max_count) == 0)		return GLFW_KEY_KP_6;
        if (tb_strncasecmp("kp_7", str, max_count) == 0)		return GLFW_KEY_KP_7;
        if (tb_strncasecmp("kp_8", str, max_count) == 0)		return GLFW_KEY_KP_8;
        if (tb_strncasecmp("kp_9", str, max_count) == 0)	    return GLFW_KEY_KP_9;
        if (tb_strncasecmp("kp_decimal", str, max_count) == 0)  return GLFW_KEY_KP_DECIMAL;
        if (tb_strncasecmp("kp_divide", str, max_count) == 0)   return GLFW_KEY_KP_DIVIDE;
        if (tb_strncasecmp("kp_multiply", str, max_count) == 0) return GLFW_KEY_KP_MULTIPLY;
        if (tb_strncasecmp("kp_subtract", str, max_count) == 0) return GLFW_KEY_KP_SUBTRACT;
        if (tb_strncasecmp("kp_add", str, max_count) == 0)		return GLFW_KEY_KP_ADD;
        if (tb_strncasecmp("kp_enter", str, max_count) == 0)	return GLFW_KEY_KP_ENTER;
        if (tb_strncasecmp("kp_equal", str, max_count) == 0)	return GLFW_KEY_KP_EQUAL;
        break;
    case 'l': case 'L':
        if (tb_strncasecmp("left", str, max_count) == 0)		 return GLFW_KEY_LEFT;
        if (tb_strncasecmp("left_bracket", str, max_count) == 0) return GLFW_KEY_LEFT_BRACKET;
        if (tb_strncasecmp("left_shift", str, max_count) == 0)   return GLFW_KEY_LEFT_SHIFT;
        if (tb_strncasecmp("left_control", str, max_count) == 0) return GLFW_KEY_LEFT_CONTROL;
        if (tb_strncasecmp("left_alt", str, max_count) == 0)	 return GLFW_KEY_LEFT_ALT;
        if (tb_strncasecmp("left_super", str, max_count) == 0)	 return GLFW_KEY_LEFT_SUPER;
        break;
    case 'm': case 'M':
        if (tb_strncasecmp("minus", str, max_count) == 0) return GLFW_KEY_MINUS;
        break;
    case 'n': case 'N':
        if (tb_strncasecmp("num_lock", str, max_count) == 0) return GLFW_KEY_NUM_LOCK;
        break;
    case 'p': case 'P':
        if (tb_strncasecmp("period", str, max_count) == 0)		 return GLFW_KEY_PERIOD;
        if (tb_strncasecmp("page_up", str, max_count) == 0)		 return GLFW_KEY_PAGE_UP;
        if (tb_strncasecmp("page_down", str, max_count) == 0)	 return GLFW_KEY_PAGE_DOWN;
        if (tb_strncasecmp("print_screen", str, max_count) == 0) return GLFW_KEY_PRINT_SCREEN;
        if (tb_strncasecmp("pause", str, max_count) == 0)		 return GLFW_KEY_PAUSE;
        break;
    case 'r': case 'R':
        if (tb_strncasecmp("right", str, max_count) == 0)		  return GLFW_KEY_RIGHT;
        if (tb_strncasecmp("right_bracket", str, max_count) == 0) return GLFW_KEY_RIGHT_BRACKET;
        if (tb_strncasecmp("right_shift", str, max_count) == 0)   return GLFW_KEY_RIGHT_SHIFT;
        if (tb_strncasecmp("right_control", str, max_count) == 0) return GLFW_KEY_RIGHT_CONTROL;
        if (tb_strncasecmp("right_alt", str, max_count) == 0)	  return GLFW_KEY_RIGHT_ALT;
        if (tb_strncasecmp("right_super", str, max_count) == 0)	  return GLFW_KEY_RIGHT_SUPER;
        break;
    case 's': case 'S':
        if (tb_strncasecmp("space", str, max_count) == 0)		return GLFW_KEY_SPACE;
        if (tb_strncasecmp("slash", str, max_count) == 0)		return GLFW_KEY_SLASH;
        if (tb_strncasecmp("semicolon", str, max_count) == 0)	return GLFW_KEY_SEMICOLON;
        if (tb_strncasecmp("scroll_lock", str, max_count) == 0) return GLFW_KEY_SCROLL_LOCK;
        break;
    case 't': case 'T':
        if (tb_strncasecmp("tab", str, max_count) == 0) return GLFW_KEY_TAB;
        break;
    case 'u': case 'U':
        if (tb_strncasecmp("up", str, max_count) == 0) return GLFW_KEY_UP;
        break;
    }

    return GLFW_KEY_UNKNOWN;
}

int FrostParseInputMouseButton(const char* str, size_t max_count)
{
    if (max_count == 0) return GLFW_KEY_UNKNOWN;

    const char* prefix = "MOUSE_BUTTON_";
    size_t prefix_len = strlen(prefix);

    if (tb_strncasecmp(prefix, str, prefix_len) == 0)
        return FrostParseInputMouseButton(str + prefix_len, max_count - prefix_len);

    switch (str[0])
    {
    case '1': return GLFW_MOUSE_BUTTON_1;
    case '2': return GLFW_MOUSE_BUTTON_2;
    case '3': return GLFW_MOUSE_BUTTON_3;
    case '4': return GLFW_MOUSE_BUTTON_4;
    case '5': return GLFW_MOUSE_BUTTON_5;
    /* named buttons */
    case 'l': case 'L':
        if (tb_strncasecmp("left", str, max_count) == 0) return GLFW_MOUSE_BUTTON_LEFT;
        break;
    case 'r': case 'R':
        if (tb_strncasecmp("right", str, max_count) == 0) return GLFW_MOUSE_BUTTON_RIGHT;
        break;
    case 'm': case 'M':
        if (tb_strncasecmp("middle", str, max_count) == 0) return GLFW_MOUSE_BUTTON_MIDDLE;
        break;
    }
    return GLFW_KEY_UNKNOWN;
}

/* ------------------| Tile |--------------------------------- */
int FrostParseTileType(const char* str, size_t max_count)
{
    if (max_count == 0) return TILE_EMPTY;
    if (tb_strncasecmp("solid", str, max_count) == 0) return TILE_SOLID;
    if (tb_strncasecmp("slope_left", str, max_count) == 0) return TILE_SLOPE_LEFT;
    if (tb_strncasecmp("slope_right", str, max_count) == 0) return TILE_SLOPE_RIGHT;
    if (tb_strncasecmp("platform", str, max_count) == 0) return TILE_PLATFORM;

    return TILE_EMPTY;
}
