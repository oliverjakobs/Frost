#ifndef FROST_H
#define FROST_H

#include "FrostEcs.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneLoader.h"
#include "Scenes/SceneEditor.h"

#include "Console/Console.h"

void FrostExecuteConsoleCommand(Console* console, Scene* scene, SceneEditor* editor, const char* cmd_buffer);

#endif /* !FROST_H */
