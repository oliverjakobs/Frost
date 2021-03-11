#ifndef FROST_H
#define FROST_H

#include "FrostEcs.h"
#include "SceneLoader.h"
#include "SceneSaver.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneEditor.h"

int FrostLoadIgnis(IgnisColorRGBA clear_color, GLenum blend_s, GLenum blend_d);
int FrostLoadScene(Scene* scene, const char* path, float w, float h);
int FrostLoadRenderer(const char* path);

#endif /* !FROST_H */
