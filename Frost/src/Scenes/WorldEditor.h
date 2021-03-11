#ifndef WORLD_EDITOR_H
#define WORLD_EDITOR_H

#include "SceneEditor.h"

void WorldEditorOnEvent(SceneEditor* editor, Event e);
void WorldEditorOnUpdate(SceneEditor* editor, float deltatime);
void WorldEditorOnRender(const SceneEditor* editor);

#endif // !WORLD_EDITOR_H