#include "Inventory.h"

#include "Frost/Frost.h"

#include "toolbox/tb_str.h"
#include "toolbox/tb_json.h"

#include "Application/Debugger.h"

#include "..\Systems\InventorySystem.h"

const float INV_CELL_SIZE = 64.0f;
const float INV_PADDING = 8.0f;

void InventoryLoad(Scene* scene, EcsEntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'inventory'");
	if (element.error == TB_JSON_OK)
	{
		Inventory inv;

		vec2 pos;
		pos.x = tb_json_float(element.value, "{'position'[0", NULL, 0.0f);
		pos.y = tb_json_float(element.value, "{'position'[1", NULL, 0.0f);

		int rows = tb_json_int(element.value, "{'grid'[0", NULL, 1);
		int cols = tb_json_int(element.value, "{'grid'[1", NULL, 1);

		InventoryState state = tb_json_parse(element.value, "{'state'", NULL, (tb_json_parse_func)InventoryParseState);
		
		InventoryCreate(&inv, state, pos, rows, cols);
		InventorySetLayout(&inv, INV_CELL_SIZE, INV_PADDING);

		tb_json_element align;
		tb_json_read(element.value, &align, "{'align'");
		if (align.error == TB_JSON_OK)
		{
			InvHAlign h_align = tb_json_parse(align.value, "[0", NULL, (tb_json_parse_func)InventoryParseHAlign);
			InvVAlign v_align = tb_json_parse(align.value, "[1", NULL, (tb_json_parse_func)InventoryParseVAlign);

			InventoryAlign(&inv, h_align, v_align, scene->camera.size);
		}

		/* TODO load inv content */
		InventorySetCellContent(&inv, 0, 3);
		InventorySetCellContent(&inv, 1, 1);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INVENTORY, &inv);
	}
}

int InventoryCreate(Inventory* inv, InventoryState state, vec2 pos, int rows, int cols)
{
	size_t size = sizeof(ItemID) * (size_t)rows * (size_t)cols;
	inv->cells = malloc(size);

	if (!inv->cells) return 0;

	memset(inv->cells, NULL_ITEM, size);

	inv->state = state;

	inv->pos = pos;

	inv->rows = rows;
	inv->cols = cols;

	return 1;
}

void InventorySetLayout(Inventory* inv, float cell_size, float padding)
{
	inv->cell_size = cell_size;
	inv->padding = padding;
}

void InventoryAlign(Inventory* inv, InvHAlign h_align, InvVAlign v_align, vec2 screen_size)
{
	float width = InventoryGetWidth(inv);
	float height = InventoryGetHeight(inv);

	switch (h_align)
	{
	case INV_HALIGN_LEFT:	inv->pos.x = -screen_size.x * 0.5f; break;
	case INV_HALIGN_CENTER:	inv->pos.x = -width * 0.5f; break;
	case INV_HALIGN_RIGHT:	inv->pos.x = (screen_size.x * 0.5f) - width; break;
	}

	switch (v_align)
	{
	case INV_VALIGN_TOP:	inv->pos.y = (screen_size.y * 0.5f) - height; break;
	case INV_VALIGN_CENTER:	inv->pos.y = -height * 0.5f; break;
	case INV_VALIGN_BOTTOM:	inv->pos.y = -screen_size.y * 0.5f; break;
	}
}

void InventoryFree(Inventory* inv)
{
	if (inv->cells) free(inv->cells);
	inv->cells = NULL;
}

void InventoryToggle(Inventory* inv)
{
	if (inv->state == INVENTORY_OPEN) inv->state = INVENTORY_CLOSED;
	else if (inv->state == INVENTORY_CLOSED) inv->state = INVENTORY_OPEN;
}

int InventoryGetCellIndex(const Inventory* inv, int row, int column)
{
	return row * inv->cols + column;
}

int InventoryGetCellAt(const Inventory* inv, vec2 pos)
{
	vec2 cell_offset = vec2_sub(pos, inv->pos);

	/* filter first padding (left or under first cells) */
	if (cell_offset.x < inv->padding || cell_offset.y < inv->padding) return -1;

	/* prevent falsely jumping to next row */
	float width = InventoryGetWidth(inv);
	if (cell_offset.x >= (width - inv->padding)) return -1;

	int col = (int)(cell_offset.x / (inv->cell_size + inv->padding));
	int row = (int)(cell_offset.y / (inv->cell_size + inv->padding));

	/* filter cells that would be out of bounds */
	if (row >= inv->rows || col >= inv->cols) return -1;

	/* filter padding between cells */
	float col_offset = InventoryGetCellOffset(inv, col);
	float row_offset = InventoryGetCellOffset(inv, row);
	if ((cell_offset.x < col_offset) || (cell_offset.y < row_offset)) return -1;

	return InventoryGetCellIndex(inv, row, col);
}

vec2 InventoryGetCellPos(const Inventory* inv, int index)
{
	int col = index % inv->cols;
	int row = index / inv->cols;

	vec2 pos;
	pos.x = InventoryGetCellOffset(inv, col);
	pos.y = InventoryGetCellOffset(inv, row);

	return vec2_add(inv->pos, pos);
}

float InventoryGetCellOffset(const Inventory* inv, int index)
{
	return index * (inv->cell_size + inv->padding) + inv->padding;
}

void InventorySetCellContent(Inventory* inv, int index, ItemID itemID)
{
	if (index < 0 || index >= (inv->rows * inv->cols)) return;

	inv->cells[index] = itemID;
}

ItemID InventoryGetCellContent(const Inventory* inv, int index)
{
	if (index < 0 || index >= (inv->rows * inv->cols)) return NULL_ITEM;

	return inv->cells[index];
}

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell)
{
	InventorySetCellContent(dst_inv, dst_cell, InventoryGetCellContent(src_inv, src_cell));
	InventorySetCellContent(src_inv, src_cell, NULL_ITEM);
}

float InventoryGetWidth(const Inventory* inv)
{
	return InventoryGetCellOffset(inv, inv->cols);
}

float InventoryGetHeight(const Inventory* inv)
{
	return InventoryGetCellOffset(inv, inv->rows);
}

InventoryState InventoryParseState(const char* str, size_t max_count)
{
	if (max_count == 0) return INVENTORY_CLOSED;
	if (tb_strncasecmp(str, "open", max_count) == 0) return INVENTORY_OPEN;
	if (tb_strncasecmp(str, "fixed", max_count) == 0) return INVENTORY_FIXED;

	return INVENTORY_CLOSED;
}

InvHAlign InventoryParseHAlign(const char* str, size_t max_count)
{
	if (max_count == 0) return INV_HALIGN_NONE;
	if (tb_strncasecmp(str, "left", max_count) == 0) return INV_HALIGN_LEFT;
	if (tb_strncasecmp(str, "center", max_count) == 0) return INV_HALIGN_CENTER;
	if (tb_strncasecmp(str, "right", max_count) == 0) return INV_HALIGN_RIGHT;

	return INV_HALIGN_NONE;
}

InvVAlign InventoryParseVAlign(const char* str, size_t max_count)
{
	if (max_count == 0) return INV_VALIGN_NONE;
	if (tb_strncasecmp(str, "top", max_count) == 0) return INV_VALIGN_TOP;
	if (tb_strncasecmp(str, "center", max_count) == 0) return INV_VALIGN_CENTER;
	if (tb_strncasecmp(str, "bottom", max_count) == 0) return INV_VALIGN_BOTTOM;

	return INV_VALIGN_NONE;
}
