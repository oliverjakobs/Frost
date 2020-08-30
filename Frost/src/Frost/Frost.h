#ifndef FROST_H
#define FROST_H

#include "Scenes/Scenes.h"
#include "Scenes/SceneRegister.h"
#include "Scenes/SceneEditor.h"

#include "Console/Console.h"

void FrostExecuteConsoleCommand(Console* console, Scenes* scenes, SceneRegister* reg, SceneEditor* editor, const char* cmd_buffer);

#endif /* !FROST_H */
