#include "TemplateLoader.h"

#include "Obelisk/FileUtility.h"
#include "Obelisk/Debugger.h"

#define JSON_IMPLEMENTATION
#include "json/json.h"

std::shared_ptr<Entity> TemplateLoader::LoadEntity(const std::string& path)
{
	std::string json = obelisk::ReadFile(path);

	json_element_t element;
	json_read(json.data(), "{'name'", &element);

	std::string name((char*)element.value, element.bytelen);
	if (name.empty()) return nullptr;

	auto entity = std::make_shared<Entity>(name);

	json_read(json.data(), "{'physics'", &element);
	if (element.error == JSON_OK)
	{
		json_element_t body;
		json_read((char*)element.value, "{'body'", &body);

		if (body.error == JSON_OK)
		{
			//BodyType type = (BodyType)json_int((char*)body.value, "{'type'", NULL);

			float x = json_float((char*)body.value, "{'position'[0", NULL);
			float y = json_float((char*)body.value, "{'position'[1", NULL);

			float w = json_float((char*)body.value, "{'halfDimension'[0", NULL);
			float h = json_float((char*)body.value, "{'halfDimension'[1", NULL);

			float offset_x = json_float((char*)element.value, "{'offset'[0", NULL);
			float offset_y = json_float((char*)element.value, "{'offset'[1", NULL);

			//entity->AddComponent<PhysicsComponent>(std::make_shared<Body>(x, y, w, h, type), glm::vec2(offset_x, offset_y));
		}
	}

	json_read(json.data(), "{'texture'", &element);
	if (element.error == JSON_OK)
	{
		json_element_t texture;
		json_read((char*)element.value, "{'src'", &texture);
		std::string src((char*)texture.value, texture.bytelen);

		int rows = json_int((char*)element.value, "{'atlas'[0", NULL);
		int columns = json_int((char*)element.value, "{'atlas'[1", NULL);

		float width = json_float((char*)element.value, "{'dimension'[0", NULL);
		float height = json_float((char*)element.value, "{'dimension'[1", NULL);

		entity->AddComponent<TextureComponent>(std::make_shared<ignis::Texture>(src, rows, columns), width, height);
	}

	json_read(json.data(), "{'animation'", &element);
	if (element.error == JSON_OK)
	{
		std::map<std::string, Animation> animations;

		for (int i = 0; i < element.elements; i++)
		{
			json_element_t anim;
			json_read_param((char*)element.value, "{*", &anim, &i);

			std::string anim_name((char*)anim.value, anim.bytelen);

			json_element_t array;
			json_read((char*)element.value, (char*)obelisk::format("{'%s'", anim_name.c_str()).c_str(), &array);
			if (array.data_type == JSON_ARRAY)
			{
				char* value = (char*)array.value;
				json_element_t array_element;

				// start
				unsigned int start;
				value = json_array_step(value, &array_element);
				json_atoi((char*)array_element.value, &start);

				// length
				unsigned int length;
				value = json_array_step(value, &array_element);
				json_atoi((char*)array_element.value, &length);

				// duration
				float duration;
				value = json_array_step(value, &array_element);
				json_atof((char*)array_element.value, &duration);

				animations.insert({ anim_name, Animation(start, length, duration) });
			}
		}
		entity->AddComponent<AnimationComponent>(animations);
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
