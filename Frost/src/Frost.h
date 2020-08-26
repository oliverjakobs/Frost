#ifndef FROST_H
#define FROST_H

#include "Scenes/SceneManager.h"
#include "Scenes/SceneLoader.h"
#include "Scenes/SceneEditor.h"

#include "Console/Console.h"

void FrostExecuteConsoleCommand(Console* console, SceneManager* manager, SceneEditor* editor, const char* cmd_buffer);

#endif /* !FROST_H */
