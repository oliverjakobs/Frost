#include "TemplateLoader.h"

#include "Obelisk/FileUtility.h"
#include "Obelisk/Debugger.h"

#define JSON_IMPLEMENTATION
#include "json/json.h"

std::shared_ptr<Entity> TemplateLoader::LoadEntity(const std::string& path)
{
	std::string json = obelisk::ReadFile(path);

	if (json.empty())
	{
		OBELISK_WARN("Template not found (%s)", path.c_str());
		return nullptr;
	}

	json_element element;
	json_read(json.data(), "{'name'", &element);

	std::string name((char*)element.value, element.bytelen);
	if (name.empty()) return nullptr;

	auto entity = std::make_shared<Entity>(name);

	json_read(json.data(), "{'position'", &element);
	if (element.error == JSON_OK)
	{
		float x = json_float((char*)element.value, "[0", NULL);
		float y = json_float((char*)element.value, "[1", NULL);

		entity->AddComponent<PositionComponent>(glm::vec2(x, y));
	}

	json_read(json.data(), "{'physics'", &element);
	if (element.error == JSON_OK)
	{
		json_element body;
		json_read((char*)element.value, "{'body'", &body);

		if (body.error == JSON_OK)
		{
			BodyType type = (BodyType)json_int((char*)body.value, "{'type'", NULL);

			float x = json_float((char*)body.value, "{'position'[0", NULL);
			float y = json_float((char*)body.value, "{'position'[1", NULL);

			float w = json_float((char*)body.value, "{'halfDimension'[0", NULL);
			float h = json_float((char*)body.value, "{'halfDimension'[1", NULL);

			float offset_x = json_float((char*)element.value, "{'offset'[0", NULL);
			float offset_y = json_float((char*)element.value, "{'offset'[1", NULL);

			entity->AddComponent<PhysicsComponent>(std::make_shared<Body>(x, y, w, h, type), glm::vec2(offset_x, offset_y));
		}
	}

	json_read(json.data(), "{'texture'", &element);
	if (element.error == JSON_OK)
	{
		json_element texture;
		json_read((char*)element.value, "{'src'", &texture);
		std::string src((char*)texture.value, texture.bytelen);

		int rows = std::max(json_int((char*)element.value, "{'atlas'[0", NULL), 1);
		int columns = std::max(json_int((char*)element.value, "{'atlas'[1", NULL), 1);

		float width = json_float((char*)element.value, "{'dimension'[0", NULL);
		float height = json_float((char*)element.value, "{'dimension'[1", NULL);

		entity->AddComponent<TextureComponent>(ignisCreateTexture(src.c_str(), rows, columns, true), width, height);
	}

	json_read(json.data(), "{'animation'", &element);
	if (element.error == JSON_OK)
	{
		auto animator = std::make_shared<Animator>();
		animator->LoadConditions();
		
		for (int i = 0; i < element.elements; i++)
		{
			json_element anim_name;
			json_read_param((char*)element.value, "{*", &anim_name, &i);

			json_element anim; 
			json_read((char*)element.value, (char*)obelisk::format("{'%.*s'", anim_name.bytelen, anim_name.value).c_str(), &anim);

			unsigned int start = json_int((char*)anim.value, "{'start'", NULL);
			unsigned int length = json_int((char*)anim.value, "{'length'", NULL);
			float delay = json_float((char*)anim.value, "{'delay'", NULL);

			json_element transition_array; 
			json_read((char*)anim.value, "{'transitions'", &transition_array);

			std::vector<Transition> transitions;

			char* value = (char*)transition_array.value;
			json_element transition_element;
			for (int i = 0; i < transition_array.elements; i++)
			{				
				value = json_array_step(value, &transition_element);

				if (transition_element.data_type == JSON_ARRAY)
				{
					json_element condition;
					json_read_param((char*)transition_element.value, "[0", &condition, NULL);
					json_element next;
					json_read_param((char*)transition_element.value, "[1", &next, NULL);

					transitions.push_back({ std::string((char*)condition.value, condition.bytelen), std::string((char*)next.value, next.bytelen) });
				}
			}

			animator->CreateAnimation(std::string((char*)anim_name.value, anim_name.bytelen), start, length, delay, transitions);
			
		}

		entity->AddComponent<AnimationComponent>(animator);
	}

	json_read(json.data(), "{'player'", &element);
	if (element.error == JSON_OK)
	{
		float ms = json_float((char*)element.value, "{'movementspeed'", NULL);
		float jp = json_float((char*)element.value, "{'jumppower'", NULL);
		
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

	float width = json_float(json.data(), "{'size'[0", NULL);
	float height = json_float(json.data(), "{'size'[1", NULL);

	auto scene = std::make_shared<Scene>(camera, width, height);

	json_element element;

	json_read(json.data(), "{'templates'", &element);
	if (element.error == JSON_OK)
	{
		for (int i = 0; i < element.elements; i++)
		{
			json_element entity;
			json_read_param((char*)element.value, "{*", &entity, &i);

			std::string templ((char*)entity.value, entity.bytelen);

			json_element positions;
			json_read((char*)element.value, (char*)obelisk::format("{'%s'", templ.c_str()).c_str(), &positions);

			if (positions.error == JSON_OK)
			{
				char* value = (char*)positions.value;
				json_element array_element;

				for (int i = 0; i < positions.elements; i++)
				{
					value = json_array_step(value, &array_element);

					float x = json_float((char*)array_element.value, "[0", NULL);
					float y = json_float((char*)array_element.value, "[1", NULL);

					scene->AddEntity(TemplateLoader::LoadEntity(templ), glm::vec2(x, y));
				}
			}
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

	json_element element;
	json_read(json.data(), "{'scenes'", &element);

	for (int i = 0; i < element.elements; i++)
	{
		json_element scene;
		json_read_param((char*)element.value, "{*", &scene, &i);

		std::string sceneName((char*)scene.value, scene.bytelen);

		json_element templ;
		json_read((char*)element.value, (char*)obelisk::format("{'%s'", sceneName.c_str()).c_str(), &templ);

		std::string templPath((char*)templ.value, templ.bytelen);

		if (!(sceneName.empty() || templPath.empty()))
			manager->RegisterScene(sceneName, templPath);
	}
}
