#include "Inventory.h"

#include "Frost/Frost.h"

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

		inv.state = (InventoryState)tb_json_int(element.value, "{'state'", NULL, INVENTORY_CLOSED);

		tb_json_element align;
		tb_json_read(element.value, &align, "{'align'");

		if (align.error == TB_JSON_OK)
		{
			char buffer[32];
			tb_json_string(align.value, "[0", buffer, 32, NULL);
			InventoryHAlign h_align = InventorySystemGetHAlign(buffer);

			tb_json_string(align.value, "[1", buffer, 32, NULL);
			InventoryVAlign v_align = InventorySystemGetVAlign(buffer);

			InventoryCreateAligned(&inv, h_align, v_align, rows, cols);
		}
		else
		{
			/* TODO position */
		}

		/* TODO load inv content */
		InventorySetCellContent(&inv, 0, 3);
		InventorySetCellContent(&inv, 1, 1);
		InventorySetCellContent(&inv, 2, 2);
		InventorySetCellContent(&inv, 3, 3);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INVENTORY, &inv);
	}
}

void InventoryFree(Inventory* inv)
{
	if (inv->cells) free(inv->cells);
	inv->cells = NULL;

	inv->pos = vec2_zero();
	inv->size = vec2_zero();

	inv->columns = inv->rows = 0;
}

void InventoryToggle(Inventory* inv)
{
	if (inv->state == INVENTORY_OPEN) inv->state = INVENTORY_CLOSED;
	else if (inv->state == INVENTORY_CLOSED) inv->state = INVENTORY_OPEN;
}

int InventoryGetCellIndex(Inventory* inv, int row, int column)
{
	return row * inv->columns + column;
}

void InventorySetCellContent(Inventory* inv, int index, int itemID)
{
	if (index < 0 || index >= (inv->rows * inv->columns)) return;

	inv->cells[index].itemID = itemID;
}

int InventoryGetCellContent(Inventory* inv, int index)
{
	if (index < 0 || index >= (inv->rows * inv->columns)) return NULL_ITEM;

	return inv->cells[index].itemID;
}

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell)
{
	InventorySetCellContent(dst_inv, dst_cell, InventoryGetCellContent(src_inv, src_cell));
	InventorySetCellContent(src_inv, src_cell, NULL_ITEM);
}