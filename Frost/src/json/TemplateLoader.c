#include "TemplateLoader.h"

#include "Debugger.h"

#define TB_JSON_IMPLEMENTATION
#include "json/tb_json.h"

#define TEMPLATE_LOADER_STRLEN	32
#define TEMPLATE_LOADER_PATHLEN	64

EcsEntity* TemplateLoadEntity(const char* json_path, ResourceManager* res)
{
	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		DEBUG_WARN("Template not found (%s)", json_path);
		return NULL;
	}

	char name[TEMPLATE_LOADER_STRLEN];
	tb_json_string(json, "{'name'", name, TEMPLATE_LOADER_STRLEN, NULL);

	EcsEntity* entity = (EcsEntity*)malloc(sizeof(EcsEntity));
	EcsEntityLoad(entity, name);

	tb_json_element element;
	tb_json_read(json, &element, "{'position'");
	if (element.error == TB_JSON_OK)
	{
		float x = tb_json_float((char*)element.value, "[0", NULL);
		float y = tb_json_float((char*)element.value, "[1", NULL);

		EcsEntityAddPosition(entity, x, y);
	}

	tb_json_read(json, &element, "{'physics'");
	if (element.error == TB_JSON_OK)
	{
		tb_json_element body;
		tb_json_read((char*)element.value, &body, "{'body'");

		if (body.error == TB_JSON_OK)
		{
			BodyType type = (BodyType)tb_json_int((char*)body.value, "{'type'", NULL);

			float x = tb_json_float((char*)body.value, "{'position'[0", NULL);
			float y = tb_json_float((char*)body.value, "{'position'[1", NULL);

			float w = tb_json_float((char*)body.value, "{'halfDimension'[0", NULL);
			float h = tb_json_float((char*)body.value, "{'halfDimension'[1", NULL);

			float offset_x = tb_json_float((char*)element.value, "{'offset'[0", NULL);
			float offset_y = tb_json_float((char*)element.value, "{'offset'[1", NULL);

			Body* body = (Body*)malloc(sizeof(Body));
			BodyLoad(body, x, y, w, h, type);

			EcsEntityAddPhysics(entity, body, offset_x, offset_y);
		}
	}

	tb_json_read(json, &element, "{'texture'");
	if (element.error == TB_JSON_OK)
	{
		char tex_name[TEMPLATE_LOADER_STRLEN];
		tb_json_string((char*)element.value, "{'name'", tex_name, TEMPLATE_LOADER_STRLEN, NULL);

		float width = tb_json_float((char*)element.value, "{'dimension'[0", NULL);
		float height = tb_json_float((char*)element.value, "{'dimension'[1", NULL);

		int frame = tb_json_int((char*)element.value, "{'frame'", NULL);

		IgnisTexture* texture = ResourceManagerGetTexture(res, tex_name);

		if (texture)
			EcsEntityAddTexture(entity, texture, width, height, frame);
	}

	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		Animator* animator = (Animator*)malloc(sizeof(Animator));
		AnimatorInit(animator);
		
		for (int i = 0; i < element.elements; i++)
		{
			char anim_name[TEMPLATE_LOADER_STRLEN];
			tb_json_string((char*)element.value, "{*", anim_name, TEMPLATE_LOADER_STRLEN, &i);
		
			tb_json_element anim; 
			tb_json_read_format((char*)element.value, &anim, "{'%s'", anim_name);
		
			unsigned int start = tb_json_int((char*)anim.value, "{'start'", NULL);
			unsigned int length = tb_json_int((char*)anim.value, "{'length'", NULL);
			float delay = tb_json_float((char*)anim.value, "{'delay'", NULL);
		
			tb_json_element transition_array; 
			tb_json_read((char*)anim.value, &transition_array, "{'transitions'");
		
			Animation* animation = (Animation*)malloc(sizeof(Animation));
			AnimationLoad(animation, start, length, delay, transition_array.elements);
		
			char* value = (char*)transition_array.value;
			tb_json_element transition_element;
			for (int i = 0; i < transition_array.elements; i++)
			{				
				value = tb_json_array_step(value, &transition_element);
		
				if (transition_element.data_type == TB_JSON_ARRAY)
				{
					char condition[TEMPLATE_LOADER_STRLEN];
					tb_json_string((char*)transition_element.value, "[0", condition, TEMPLATE_LOADER_STRLEN, NULL);
					char next[TEMPLATE_LOADER_STRLEN];
					tb_json_string((char*)transition_element.value, "[1", next, TEMPLATE_LOADER_STRLEN, NULL);
		
					AnimationAddTransition(animation, condition, next);
				}
			}
		
			AnimatorAddAnimation(animator, anim_name, animation);
		}
		/* AnimatorDebugPrint(animator); */
		EcsEntityAddAnimation(entity, animator);
	}

	tb_json_read(json, &element, "{'movement'");
	if (element.error == TB_JSON_OK)
	{
		float ms = tb_json_float((char*)element.value, "{'speed'", NULL);
		float jp = tb_json_float((char*)element.value, "{'jumppower'", NULL);
		
		EcsEntityAddMovement(entity, ms, jp);
	}

	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		float smooth = tb_json_float((char*)element.value, "{'smooth'", NULL);

		EcsEntityAddCamera(entity, smooth);
	}

	free(json);

	return entity;
}