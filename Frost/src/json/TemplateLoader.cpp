#include "TemplateLoader.hpp"

#include "Obelisk/Debugger.hpp"

#define TB_JSON_IMPLEMENTATION
#include "json/tb_json.h"

#define TEMPLATE_LOADER_STRLEN	32
#define TEMPLATE_LOADER_PATHLEN	64

std::shared_ptr<Entity> TemplateLoadEntity(const char* json_path, ResourceManager* res)
{
	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		OBELISK_WARN("Template not found (%s)", json_path);
		return nullptr;
	}

	char name[TEMPLATE_LOADER_STRLEN];
	tb_json_string(json, "{'name'", name, TEMPLATE_LOADER_STRLEN, NULL);

	auto entity = std::make_shared<Entity>(name);

	tb_json_element element;
	tb_json_read(json, &element, "{'position'");
	if (element.error == TB_JSON_OK)
	{
		float x = tb_json_float((char*)element.value, "[0", NULL);
		float y = tb_json_float((char*)element.value, "[1", NULL);

		entity->AddComponent<PositionComponent>(glm::vec2(x, y));
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

			entity->AddComponent<PhysicsComponent>(std::make_shared<Body>(x, y, w, h, type), glm::vec2(offset_x, offset_y));
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
			entity->AddComponent<TextureComponent>(texture, width, height, frame);
	}

	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		auto animator = std::make_shared<Animator>();
		animator->LoadConditions();
		
		for (int i = 0; i < element.elements; i++)
		{
			tb_json_element anim_name;
			tb_json_read_param((char*)element.value, &anim_name, "{*", &i);

			tb_json_element anim; 
			tb_json_read_format((char*)element.value, &anim, "{'%.*s'", anim_name.bytelen, anim_name.value);

			unsigned int start = tb_json_int((char*)anim.value, "{'start'", NULL);
			unsigned int length = tb_json_int((char*)anim.value, "{'length'", NULL);
			float delay = tb_json_float((char*)anim.value, "{'delay'", NULL);

			tb_json_element transition_array; 
			tb_json_read((char*)anim.value, &transition_array, "{'transitions'");

			std::vector<Transition> transitions;

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

					transitions.push_back({ condition, next });
				}
			}
			animator->CreateAnimation(std::string((char*)anim_name.value, anim_name.bytelen), start, length, delay, transitions);
		}

		entity->AddComponent<AnimationComponent>(animator);
	}

	tb_json_read(json, &element, "{'player'");
	if (element.error == TB_JSON_OK)
	{
		float ms = tb_json_float((char*)element.value, "{'movementspeed'", NULL);
		float jp = tb_json_float((char*)element.value, "{'jumppower'", NULL);
		
		entity->AddComponent<PlayerComponent>(ms, jp);
	}

	free(json);

	return entity;
}

std::shared_ptr<Scene> TemplateLoadScene(const char* json_path, Camera* camera, ResourceManager* res)
{
	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		OBELISK_WARN("Template not found (%s)", json_path);
		return nullptr;
	}

	float width = tb_json_float(json, "{'size'[0", NULL);
	float height = tb_json_float(json, "{'size'[1", NULL);

	auto scene = std::make_shared<Scene>(camera, width, height);

	tb_json_element templates;
	tb_json_read(json, &templates, "{'templates'");

	if (templates.error == TB_JSON_OK && templates.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)templates.value;
		tb_json_element entity;

		for (int i = 0; i < templates.elements; i++)
		{
			value = tb_json_array_step(value, &entity);

			char path[TEMPLATE_LOADER_PATHLEN];
			tb_json_string((char*)entity.value, "[0", path, TEMPLATE_LOADER_PATHLEN, NULL);

			float x = tb_json_float((char*)entity.value, "[1[0", NULL);
			float y = tb_json_float((char*)entity.value, "[1[1", NULL);

			int layer = tb_json_int((char*)entity.value, "[2", NULL);

			scene->AddEntity(TemplateLoadEntity(path, res), layer, glm::vec2(x, y));
		}
	}

	free(json);

	return scene;
}

void TemplateLoadSceneRegister(SceneManager* manager, const char* json_path)
{
	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		OBELISK_WARN("Register not found (%s)", json_path);
		return;
	}

	tb_json_element scenes;
	tb_json_read(json, &scenes, "{'scenes'");

	for (int i = 0; i < scenes.elements; i++)
	{
		char name[TEMPLATE_LOADER_STRLEN];
		tb_json_string((char*)scenes.value, "{*", name, TEMPLATE_LOADER_STRLEN, &i);

		tb_json_element scene;
		tb_json_read_format((char*)scenes.value, &scene, "{'%s'", name);

		char path[TEMPLATE_LOADER_PATHLEN];
		strncpy(path, (char*)scene.value, scene.bytelen);

		path[scene.bytelen] = '\0';

		SceneManagerRegisterScene(manager, name, path);
	}

	free(json);
}