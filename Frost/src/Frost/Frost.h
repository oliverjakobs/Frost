#ifndef FROST_H
#define FROST_H

#include "Scenes/Scene.h"
#include "Scenes/SceneEditor.h"

#include "Console/Console.h"

void FrostExecuteConsoleCommand(Console* console, Scene* scene, SceneEditor* editor, const char* cmd_buffer);


/* ----------------------| ECS UTILITY |------------------------------ */
#include "EcsLoader.h"

EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos);

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(Ecs* ecs, EcsEntityID entity);

#endif /* !FROST_H */
