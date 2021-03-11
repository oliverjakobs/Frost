#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "SceneEditor.h"

void MapEditorOnEvent(SceneEditor* editor, Event e);
void MapEditorOnUpdate(SceneEditor* editor, float deltatime);
void MapEditorOnRender(const SceneEditor* editor);

#endif // !MAP_EDITOR_H
