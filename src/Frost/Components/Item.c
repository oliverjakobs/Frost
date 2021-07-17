#include "Item.h"

#include "Frost/FrostParser.h"

void ItemLoad(char* ini, Ecs* ecs, EcsEntityID entity, int variant)
{
	tb_ini_element element;
	tb_ini_query(ini, "item", NULL, &element);
	if (element.error == TB_INI_OK)
	{
		Item item;
		item.id = FrostMatchVariantINI(element.start, NULL, "id", variant, 0);

		EcsAddDataComponent(ecs, entity, COMPONENT_ITEM, &item);
	}
}