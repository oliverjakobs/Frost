#ifndef FROST_H
#define FROST_H

#include "Scenes/Scene.h"
#include "Scenes/SceneEditor.h"

#include "Console/Console.h"

void FrostExecuteConsoleCommand(Console* console, Scene* scene, SceneEditor* editor, const char* cmd_buffer);

#endif /* !FROST_H */
