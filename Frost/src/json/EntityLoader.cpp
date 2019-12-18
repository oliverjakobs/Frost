#include "EntityLoader.h"

#include "Obelisk/FileUtility.h"
#include "Obelisk/Debugger.h"

#define JSON_IMPLEMENTATION
#include "json/json.h"

std::shared_ptr<Entity> EntityLoader::LoadTemplate(const std::string& path)
{
	// player
	//auto entity = std::make_shared<Entity>("player", glm::vec2(400.0f, 200.0f), glm::vec2(20.0f, 20.0f));
	//
	//entity->AddComponent<PhysicsComponent>(m_scene->GetWorld()->CreateBody(400.0f, 200.0f, 20.0f, 30.0f, BodyType::BODY_DYNAMIC), glm::vec2(0.0f, 30.0f));
	//entity->AddComponent<TextureComponent>(std::make_shared<Texture>("res/textures/player.png", 4, 6), 40.0f, 60.0f);
	//
	//std::map<std::string, Animation> animations
	//{
	//	{ "idle", Animation(0, 4, 0.2f) },
	//	{ "walk", Animation(6, 6, 0.125f) },
	//	{ "jump", Animation(12, 3, 0.3f) },
	//	{ "fall", Animation(18, 2, 0.4f) }
	//};
	//
	//entity->AddComponent<AnimationComponent>(animations);
	//entity->AddComponent<PlayerComponent>(400.0f, 600.0f);

	std::string json = obelisk::ReadFile(path);

	char str[32];
	json_string(json.data(), "{'name'", str, 32, NULL);

	std::string name = str;
	if (name.empty()) return nullptr;

	float x = json_double(json.data(), "{'position' [0", NULL);
	float y = json_double(json.data(), "{'position' [1", NULL);

	auto entity = std::make_shared<Entity>(name, glm::vec2(x, y));

	json_element_t element;

	json_read(json.data(), "{'physics'", &element);
	if (element.error == JSON_OK)
	{
		OBELISK_INFO("Has physics component");
	}

	json_read(json.data(), "{'texture'", &element);
	if (element.error == JSON_OK)
	{
		OBELISK_INFO("Has texture component");
	}

	json_read(json.data(), "{'animation'", &element);
	if (element.error == JSON_OK)
	{
		std::map<std::string, Animation> animations;

		for (int i = 0; i < element.elements; i++)
		{
			json_element_t anim;
			json_read_param(json.data(), "{'animation'{*", &anim, &i);

			std::string anim_name((char*)anim.value, anim.bytelen);

			json_element_t array;
			json_read(json.data(), (char*)obelisk::format("{'animation'{'%s'", anim_name.c_str()).c_str(), &array);
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
				double duration;
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
		OBELISK_INFO("Has player component");
	}

	return entity;
}
