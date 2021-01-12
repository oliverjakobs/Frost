#include "Inventory.h"

#include "Frost/Frost.h"

#include "toolbox/tb_str.h"
#include "toolbox/tb_json.h"

#include "Application/Debugger.h"

void InventoryLoad(Scene* scene, EcsEntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'inventory'");
	if (element.error == TB_JSON_OK)
	{
		Inventory inv;

		int rows = tb_json_int(element.value, "{'grid'[0", NULL, 1);
		int cols = tb_json_int(element.value, "{'grid'[1", NULL, 1);

		inv.state = tb_json_parse(element.value, "{'state'", NULL, (tb_json_parse_func)InventoryParseState);

		tb_json_element align;
		tb_json_read(element.value, &align, "{'align'");

		if (align.error == TB_JSON_OK)
		{
			InventoryHAlign h_align = tb_json_parse(align.value, "[0", NULL, (tb_json_parse_func)InventoryParseHAlign);
			InventoryVAlign v_align = tb_json_parse(align.value, "[1", NULL, (tb_json_parse_func)InventoryParseVAlign);

			InventoryCreateAligned(&inv, h_align, v_align, rows, cols);
		}
		else
		{
			/* TODO position */
		}

		/* TODO load inv content */
		InventorySetCellContent(&inv, 0, 3);
		InventorySetCellContent(&inv, 1, 1);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INVENTORY, &inv);
	}
}

void InventoryFree(Inventory* inv)
{
	if (inv->cells) free(inv->cells);
	inv->cells = NULL;

	inv->pos = vec2_zero();
	inv->size = vec2_zero();

	inv->cols = inv->rows = 0;
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

vec2 InventoryGetCellPos(const Inventory* inv, int index, float cell_size, float padding)
{
	int col = index % inv->cols;
	int row = index / inv->cols;

	vec2 pos;
	pos.x = col * (cell_size + padding) + padding;
	pos.y = row * (cell_size + padding) + padding;

	return vec2_add(inv->pos, pos);
}

void InventorySetCellContent(Inventory* inv, int index, int itemID)
{
	if (index < 0 || index >= (inv->rows * inv->cols)) return;

	inv->cells[index].itemID = itemID;
}

int InventoryGetCellContent(const Inventory* inv, int index)
{
	if (index < 0 || index >= (inv->rows * inv->cols)) return NULL_ITEM;

	return inv->cells[index].itemID;
}

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell)
{
	InventorySetCellContent(dst_inv, dst_cell, InventoryGetCellContent(src_inv, src_cell));
	InventorySetCellContent(src_inv, src_cell, NULL_ITEM);
}

InventoryState InventoryParseState(const char* str, size_t max_count)
{
	if (max_count == 0) return INVENTORY_CLOSED;
	if (tb_strncasecmp(str, "open", max_count) == 0) return INVENTORY_OPEN;
	if (tb_strncasecmp(str, "fixed", max_count) == 0) return INVENTORY_FIXED;

	return INVENTORY_CLOSED;
}

InventoryHAlign InventoryParseHAlign(const char* str, size_t max_count)
{
	if (max_count == 0) return INV_HALIGN_LEFT;
	if (tb_strncasecmp(str, "center", max_count) == 0) return INV_HALIGN_CENTER;
	if (tb_strncasecmp(str, "right", max_count) == 0) return INV_HALIGN_RIGHT;

	return INV_HALIGN_LEFT;
}

InventoryVAlign InventoryParseVAlign(const char* str, size_t max_count)
{
	if (max_count == 0) return INV_VALIGN_TOP;
	if (tb_strncasecmp(str, "center", max_count) == 0) return INV_VALIGN_CENTER;
	if (tb_strncasecmp(str, "bottom", max_count) == 0) return INV_VALIGN_BOTTOM;

	return INV_VALIGN_TOP;
}
