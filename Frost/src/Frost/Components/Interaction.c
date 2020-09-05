#include "Interaction.h"

#include "Frost/FrostEcs.h"

#include "json/tb_json.h"

void InteractorLoad(Scene* scene, EntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'interactor'");
	if (element.error == TB_JSON_OK)
	{
		Interactor comp;
		comp.type = (InteractionType)tb_json_int((char*)element.value, "{'type'", NULL, 0);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INTERACTOR, &comp);
	}
}

void InteractionLoad(Scene* scene, EntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'interaction'");
	if (element.error == TB_JSON_OK)
	{
		Interaction comp;
		comp.radius = tb_json_float((char*)element.value, "{'radius'", NULL, 0.0f);
		comp.type = (InteractionType)tb_json_int((char*)element.value, "{'type'", NULL, 0);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INTERACTION, &comp);
	}
}