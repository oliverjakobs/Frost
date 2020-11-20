#include "Frost.h"

#include "Console/Command.h"

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE,
	CONSOLE_CMD_LIST,
	CONSOLE_CMD_REMOVE,
	CONSOLE_CMD_SAVE
} ConsoleCmd;

static ConsoleCmd _CmdGetType(const char* buffer)
{
	ConsoleCmd cmd = CONSOLE_CMD_NONE;
	size_t cmd_length = strlen(buffer);

	switch (buffer[0])
	{
	case 'c':
		if (cmd_length > 1)
			switch (buffer[1])
			{
			case 'h': cmd = cmd_check_keyword(buffer, "change", CONSOLE_CMD_CHANGE); break;
			case 'r': cmd = cmd_check_keyword(buffer, "create", CONSOLE_CMD_CREATE); break;
			}
		break;
	case 'l': cmd = cmd_check_keyword(buffer, "list", CONSOLE_CMD_LIST); break;
	case 'r': cmd = cmd_check_keyword(buffer, "remove", CONSOLE_CMD_REMOVE); break;
	case 's': cmd = cmd_check_keyword(buffer, "save", CONSOLE_CMD_SAVE); break;
	}

	return cmd;
}

void FrostExecuteConsoleCommand(Console* console, Scene* scene, SceneEditor* editor, const char* cmd_buffer)
{
	ConsoleCmd cmd = _CmdGetType(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneChangeActive(scene, args[0]);
			SceneEditorReset(editor);
			ConsoleOut(console, "Changed Scene to %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_CREATE:
	{
		char* args[2];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 2);

		if (!spec) break;

		if (strcmp(spec, "entity") == 0)
		{
			if (!editor->active)
			{
				ConsoleOut(console, "Editmode needs to be active to create an entity.");
				break;
			}

			vec2 pos = CameraGetMousePosView(scene->camera, InputMousePositionVec2());

			if (SceneLoadTemplate(scene, args[0], EcsEntityGetNextID(), pos, atoi(args[1])))
				ConsoleOut(console, "Created entity with template %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_LIST:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		ConsoleOut(console, "%s", cmd_buffer);

		if (strcmp(spec, "scenes") == 0)
		{
			for (tb_hashmap_iter* iter = tb_hashmap_iterator(&scene->scene_register); iter; iter = tb_hashmap_iter_next(&scene->scene_register, iter))
			{
				const char* name = tb_hashmap_iter_get_key(iter);

				ConsoleOut(console, " - %s %s", name, (strcmp(name, scene->name) == 0) ? "(active)" : "");
			}
		}
		else if (strcmp(spec, "entities") == 0)
		{
			for (size_t i = 0; i < EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE)->list.used; ++i)
			{
				Template* templ = EcsGetOrderComponent(&scene->ecs, i, COMPONENT_TEMPLATE);

				if (templ)
					ConsoleOut(console, " - %d \t | %s", *(EcsEntityID*)templ, templ->templ);
			}
		}
		else if (strcmp(spec, "templates") == 0)
		{
			for (tb_hashmap_iter* iter = tb_hashmap_iterator(&scene->templates); iter; iter = tb_hashmap_iter_next(&scene->templates, iter))
			{
				const char* name = tb_hashmap_iter_get_key(iter);
				char* templ = tb_hashmap_iter_get_value(iter);

				ConsoleOut(console, " - %s: %s", name, templ);
			}
		}
		else if (strcmp(spec, "res") == 0)
		{
			ConsoleOut(console, "Textures:");
			for (tb_hashmap_iter* iter = tb_hashmap_iterator(&scene->resources->textures); iter; iter = tb_hashmap_iter_next(&scene->resources->textures, iter))
			{
				ConsoleOut(console, " - %s", tb_hashmap_iter_get_key(iter));
			}

			ConsoleOut(console, "Fonts:");
			for (tb_hashmap_iter* iter = tb_hashmap_iterator(&scene->resources->fonts); iter; iter = tb_hashmap_iter_next(&scene->resources->fonts, iter))
			{
				ConsoleOut(console, " - %s", tb_hashmap_iter_get_key(iter));
			}
		}
		break;
	}
	case CONSOLE_CMD_REMOVE:
		ConsoleOut(console, " > remove");
		break;
	case CONSOLE_CMD_SAVE:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			char* path = tb_hashmap_find(&scene->scene_register, scene->name);
			if (!path)
			{
				ConsoleOut(console, "Couldn't find path for %s", scene->name);
				break;
			}

			SceneSave(scene, path);
			ConsoleOut(console, "Saved scene (%s) to %s", scene->name, path);
		}
		break;
	}
	default:
		ConsoleOut(console, "Unkown command");
		break;
	}

	ConsoleToggleFocus(console);
}

EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Transform* transform = EcsComponentMapIterValue(iter);

		vec2 min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		vec2 max = vec2_add(min, transform->size);

		if (vec2_inside(pos, min, max))
			return EcsComponentMapIterKey(iter);
	}

	return ECS_NULL_ENTITY;
}

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) transform->position = pos;

	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (body) body->position = vec2_add(pos, body->offset);
}

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) return transform->position;

	return vec2_zero();
}

vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);

	if (transform)
	{
		vec2 center;
		center.x = transform->position.x;
		center.y = transform->position.y + (transform->size.y / 2.0f);

		return center;
	}

	return vec2_zero();
}

rect GetEntityRect(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);

	rect r;
	r.min = vec2_zero();
	r.max = vec2_zero();

	if (transform)
	{
		r.min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		r.max = vec2_add(r.min, transform->size);
	}

	return r;
}