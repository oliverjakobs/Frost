#include "Item.h"

#include "Frost/FrostParser.h"

void ItemLoad(char* ini, Scene* scene, EcsEntityID entity, int variant)
{
    tb_ini_element element;
    tb_ini_query(ini, "item", NULL, &element);
    if (element.error == TB_INI_OK)
    {
        Item item;
        item.id = FrostMatchVariant(element.start, NULL, "id", variant, NULL_ITEM);

        EcsAddDataComponent(&scene->ecs, entity, COMPONENT_ITEM, &item);
    }
}