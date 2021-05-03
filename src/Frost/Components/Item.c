#include "Item.h"

#include "Frost/FrostParser.h"

void ItemLoad(char* json, Ecs* ecs, EcsEntityID entity, int variant)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'item'");
	if (element.error == TB_JSON_OK)
	{
		Item item;
		item.id = FrostMatchVariant(element.value, "{'id'", variant);

		EcsAddDataComponent(ecs, entity, COMPONENT_ITEM, &item);
	}
}