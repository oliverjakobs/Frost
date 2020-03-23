#include "TemplateLoader.hpp"

#include "Obelisk/FileUtility.hpp"
#include "Obelisk/Debugger.hpp"

#define TB_JSON_IMPLEMENTATION
#include "json/tb_json.h"

std::shared_ptr<Entity> TemplateLoader::LoadEntity(const std::string& path)
{
	std::string json = obelisk::ReadFile(path);

	if (json.empty())
	{
		OBELISK_WARN("Template not found (%s)", path.c_str());
		return nullptr;
	}

	tb_json_element element;
	tb_json_read(json.data(), "{'name'", &element);

	std::string name((char*)element.value, element.bytelen);
	if (name.empty()) return nullptr;

	auto entity = std::make_shared<Entity>(name);

	tb_json_read(json.data(), "{'position'", &element);
	if (element.error == TB_JSON_OK)
	{
		float x = tb_json_float((char*)element.value, "[0", NULL);
		float y = tb_json_float((char*)element.value, "[1", NULL);

		entity->AddComponent<PositionComponent>(glm::vec2(x, y));
	}

	tb_json_read(json.data(), "{'physics'", &element);
	if (element.error == TB_JSON_OK)
	{
		tb_json_element body;
		tb_json_read((char*)element.value, "{'body'", &body);

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

	tb_json_read(json.data(), "{'texture'", &element);
	if (element.error == TB_JSON_OK)
	{
		tb_json_element texture;
		tb_json_read((char*)element.value, "{'src'", &texture);
		std::string src((char*)texture.value, texture.bytelen);

		int rows = std::max(tb_json_int((char*)element.value, "{'atlas'[0", NULL), 1);
		int columns = std::max(tb_json_int((char*)element.value, "{'atlas'[1", NULL), 1);

		float width = tb_json_float((char*)element.value, "{'dimension'[0", NULL);
		float height = tb_json_float((char*)element.value, "{'dimension'[1", NULL);

		entity->AddComponent<TextureComponent>(ignisCreateTexture(src.c_str(), rows, columns, true), width, height);
	}

	tb_json_read(json.data(), "{'animation'", &element);
	if (element.error == TB_JSON_OK)
	{
		auto animator = std::make_shared<Animator>();
		animator->LoadConditions();
		
		for (int i = 0; i < element.elements; i++)
		{
			tb_json_element anim_name;
			tb_json_read_param((char*)element.value, "{*", &anim_name, &i);

			tb_json_element anim; 
			tb_json_read((char*)element.value, (char*)obelisk::format("{'%.*s'", anim_name.bytelen, anim_name.value).c_str(), &anim);

			unsigned int start = tb_json_int((char*)anim.value, "{'start'", NULL);
			unsigned int length = tb_json_int((char*)anim.value, "{'length'", NULL);
			float delay = tb_json_float((char*)anim.value, "{'delay'", NULL);

			tb_json_element transition_array; 
			tb_json_read((char*)anim.value, "{'transitions'", &transition_array);

			std::vector<Transition> transitions;

			char* value = (char*)transition_array.value;
			tb_json_element transition_element;
			for (int i = 0; i < transition_array.elements; i++)
			{				
				value = tb_json_array_step(value, &transition_element);

				if (transition_element.data_type == TB_JSON_ARRAY)
				{
					tb_json_element condition;
					tb_json_read_param((char*)transition_element.value, "[0", &condition, NULL);
					tb_json_element next;
					tb_json_read_param((char*)transition_element.value, "[1", &next, NULL);

					transitions.push_back({ std::string((char*)condition.value, condition.bytelen), std::string((char*)next.value, next.bytelen) });
				}
			}

			animator->CreateAnimation(std::string((char*)anim_name.value, anim_name.bytelen), start, length, delay, transitions);
			
		}

		entity->AddComponent<AnimationComponent>(animator);
	}

	tb_json_read(json.data(), "{'player'", &element);
	if (element.error == TB_JSON_OK)
	{
		float ms = tb_json_float((char*)element.value, "{'movementspeed'", NULL);
		float jp = tb_json_float((char*)element.value, "{'jumppower'", NULL);
		
		entity->AddComponent<PlayerComponent>(ms, jp);
	}

	return entity;
}

std::shared_ptr<Scene> TemplateLoader::LoadScene(const std::string& path, std::shared_ptr<Camera> camera)
{
	std::string json = obelisk::ReadFile(path);

	if (json.empty())
	{
		OBELISK_WARN("Template not found (%s)", path.c_str());
		return nullptr;
	}

	float width = tb_json_float(json.data(), "{'size'[0", NULL);
	float height = tb_json_float(json.data(), "{'size'[1", NULL);

	auto scene = std::make_shared<Scene>(camera, width, height);

	tb_json_element element;
	tb_json_read(json.data(), "{'templates'", &element);

	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)element.value;
		tb_json_element entity;

		for (int i = 0; i < element.elements; i++)
		{
			value = tb_json_array_step(value, &entity);

			tb_json_element path;
			tb_json_read((char*)entity.value, "[0", &path);

			float x = tb_json_float((char*)entity.value, "[1[0", NULL);
			float y = tb_json_float((char*)entity.value, "[1[1", NULL);

			int layer = tb_json_int((char*)entity.value, "[2", NULL);

			scene->AddEntity(TemplateLoader::LoadEntity(std::string((char*)path.value, path.bytelen)), layer, glm::vec2(x, y));
		}
	}

	return scene;
}

void TemplateLoader::LoadSceneRegister(SceneManager* manager, const std::string& path)
{
	std::string json = obelisk::ReadFile(path);

	if (json.empty())
	{
		OBELISK_WARN("Register not found (%s)", path.c_str());
		return;
	}

	tb_json_element element;
	tb_json_read(json.data(), "{'scenes'", &element);

	for (int i = 0; i < element.elements; i++)
	{
		tb_json_element scene;
		tb_json_read_param((char*)element.value, "{*", &scene, &i);

		std::string sceneName((char*)scene.value, scene.bytelen);

		tb_json_element templ;
		tb_json_read((char*)element.value, (char*)obelisk::format("{'%s'", sceneName.c_str()).c_str(), &templ);

		std::string templPath((char*)templ.value, templ.bytelen);

		if (!(sceneName.empty() || templPath.empty()))
			manager->RegisterScene(sceneName, templPath);
	}
}
