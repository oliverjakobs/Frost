#include "Inventory.h"

#include "Frost/FrostParser.h"

const float INV_CELL_SIZE = 64.0f;
const float INV_PADDING = 8.0f;
const float INV_SPACING = 8.0f;

static void InventoryAlign(Inventory* inv, InventoryAlignH align_h, InventoryAlignV align_v, vec2 screen_size)
{
    float width = InventoryGetWidth(inv);
    float height = InventoryGetHeight(inv);

    switch (align_h)
    {
    case INV_ALIGN_H_LEFT:   inv->pos.x = 0.0f; break;
    case INV_ALIGN_H_CENTER: inv->pos.x = (screen_size.x - width) * 0.5f; break;
    case INV_ALIGN_H_RIGHT:  inv->pos.x = screen_size.x - width; break;
    }

    switch (align_v)
    {
    case INV_ALIGN_V_TOP:    inv->pos.y = screen_size.y - height; break;
    case INV_ALIGN_V_CENTER: inv->pos.y = (screen_size.y - height) * 0.5f; break;
    case INV_ALIGN_V_BOTTOM: inv->pos.y = 0.0f; break;
    }
}

static void InventoryLoadContent(Inventory* inv, const char* path)
{
    /* TODO load inv content from file */
    InventorySetCellContent(inv, 0, 3);
    InventorySetCellContent(inv, 1, 1);
}

void InventoryLoad(char* ini, Ecs* ecs, EcsEntityID entity, vec2 screen_size)
{
    tb_ini_element element;
    tb_ini_query(ini, "inventory", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        Inventory inv;

        inv.pos.x = tb_ini_float(element.start, NULL, "x", 0.0f);
        inv.pos.y = tb_ini_float(element.start, NULL, "y", 0.0f);

        inv.rows = tb_ini_int(element.start, NULL, "rows", 1);
        inv.cols = tb_ini_int(element.start, NULL, "cols", 1);

        inv.state = tb_ini_parse(element.start, NULL, "state", FrostParseInventoryState);

        size_t size = sizeof(ItemID) * (size_t)inv.rows * (size_t)inv.cols;
        inv.cells = FrostMalloc(size);
        if (!inv.cells) return;

        memset(inv.cells, NULL_ITEM, size);

        /* set layout */
        inv.cell_size = INV_CELL_SIZE;
        inv.padding = INV_PADDING;
        inv.spacing = INV_SPACING;

        /* align */
        InventoryAlignH align_h = tb_ini_parse(element.start, NULL, "align_h", FrostParseInventoryAlignH);
        InventoryAlignV align_v = tb_ini_parse(element.start, NULL, "align_v", FrostParseInventoryAlignV);

        InventoryAlign(&inv, align_h, align_v, screen_size);

        InventoryLoadContent(&inv, "");

        EcsAddDataComponent(ecs, entity, COMPONENT_INVENTORY, &inv);
    }
}

void InventoryRelease(Inventory* inv)
{
    if (inv->cells) FrostFree(inv->cells);
    inv->cells = NULL;
}

void InventoryToggle(Inventory* inv)
{
    if (inv->state == INVENTORY_OPEN) inv->state = INVENTORY_CLOSED;
    else if (inv->state == INVENTORY_CLOSED) inv->state = INVENTORY_OPEN;
}

int InventoryClampOffset(const Inventory* inv, float offset)
{
    return (int)((offset - inv->padding + inv->spacing) / (inv->cell_size + inv->spacing));;
}

int InventoryGetCellIndex(const Inventory* inv, int row, int col)
{
    if (row >= inv->rows || col >= inv->cols) return 0;
    return row * inv->cols + col;
}

int InventoryGetCellAt(const Inventory* inv, vec2 pos)
{
    vec2 cell_offset = vec2_sub(pos, inv->pos);

    /* filter first padding (left or under first cells) */
    if (cell_offset.x < inv->padding || cell_offset.y < inv->padding) return -1;

    /* prevent falsely jumping to next row */
    float width = InventoryGetWidth(inv);
    if (cell_offset.x >= (width - inv->padding)) return -1;

    int col = InventoryClampOffset(inv, cell_offset.x);
    int row = InventoryClampOffset(inv, cell_offset.y);

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
    if (index < 0 || index >= (inv->rows * inv->cols)) return vec2_zero();
    int col = index % inv->cols;
    int row = index / inv->cols;

    vec2 pos;
    pos.x = inv->pos.x + InventoryGetCellOffset(inv, col);
    pos.y = inv->pos.y + InventoryGetCellOffset(inv, row);

    return pos;
}

float InventoryGetCellOffset(const Inventory* inv, int index)
{
    if (index < 0 || index >= (inv->rows * inv->cols)) return 0.0f;
    return index * (inv->cell_size + inv->spacing) + inv->padding;
}

void InventorySetCellContent(Inventory* inv, int index, ItemID itemID)
{
    if (index >= 0 || index < (inv->rows * inv->cols)) inv->cells[index] = itemID;
}

ItemID InventoryGetCellContent(const Inventory* inv, int index)
{
    if (index >= 0 || index < (inv->rows * inv->cols)) return inv->cells[index];
    return NULL_ITEM;
}

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell)
{
    if (dst_cell < 0 || dst_cell >= (dst_inv->rows * dst_inv->cols)) return;
    if (src_cell < 0 || src_cell >= (src_inv->rows * src_inv->cols)) return;

    InventorySetCellContent(dst_inv, dst_cell, InventoryGetCellContent(src_inv, src_cell));
    InventorySetCellContent(src_inv, src_cell, NULL_ITEM);
}

float InventoryGetWidth(const Inventory* inv) 
{
    return (inv->cols * inv->cell_size) + ((inv->cols - 1) * inv->spacing) + (2.0f * inv->padding);
}

float InventoryGetHeight(const Inventory* inv)
{
    return (inv->rows * inv->cell_size) + ((inv->rows - 1) * inv->spacing) + (2.0f * inv->padding);
}
