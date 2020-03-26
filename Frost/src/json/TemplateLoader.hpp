#pragma once

#include "SceneManager.hpp"
#include "ResourceManager.h"

std::shared_ptr<Entity> TemplateLoadEntity(const char* path, ResourceManager* res);
std::shared_ptr<Scene> TemplateLoadScene(const char* path, Camera* camera, ResourceManager* res);

void TemplateLoadSceneRegister(SceneManager* manager, const char* path);
