#include "Frost/Frost.h"
#include "Frost/FrostDebugger.h"

Scene scene;
SceneEditor scene_editor;

FrostDebugger debugger;

int OnLoad(MinimalApp* app, uint32_t w, uint32_t h)
{
    /* load ignis */
    FrostLoadGraphics(IGNIS_DARK_GREY, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* init gui manager */
    FrostLoadGui(w, h, "gui");

    FrostLoadScene(&scene, w, h, "res/scenes/scene.ini");
    FrostDebuggerInit(&debugger, 1, GuiGetFont("gui"));
    FrostDebuggerBindScene(&debugger, &scene, &scene_editor);

    SceneEditorInit(&scene_editor, &scene, 400.0f, 4, SCENE_EDIT_NONE);

    /* temp event registration */
    EcsEventSubscribe(&scene.ecs, INTERACTION_TOGGLE_DOOR, InteractionToggleDoor);
    EcsEventSubscribe(&scene.ecs, INTERACTION_OPEN_INVENTORY, InteractionOpenInventory);

    return MINIMAL_OK;
}

void OnDestroy(MinimalApp* app)
{
    SceneDestroy(&scene);
    FrostDestroyGui();
    FrostDestroyGraphics();
}

int OnEvent(MinimalApp* app, const MinimalEvent* e)
{
    float w, h;
    if (minimalEventWindowSize(e, &w, &h))
    {
        GuiSetViewport(w, h);
        CameraSetProjectionOrtho(&scene.camera, w, h);
        glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    }

    switch (minimalEventKeyPressed(e))
    {
    case GLFW_KEY_ESCAPE:    minimalClose(app); break;
    case GLFW_KEY_F1:        SceneEditorToggleWorldMode(&scene_editor); break;
    case GLFW_KEY_F2:        SceneEditorToggleMapMode(&scene_editor); break;
    case GLFW_KEY_F3:        ConsoleToggleFocus(&debugger.console); break;
    case GLFW_KEY_F6:        minimalToggleVsync(app); break;
    case GLFW_KEY_F7:        minimalToggleDebug(app); break;
    case GLFW_KEY_F8:        SceneEditorToggleGrid(&scene_editor); break;
    case GLFW_KEY_F9:        FrostDebuggerToggleInfo(&debugger); break;
    }

    if (scene_editor.mode == SCENE_EDIT_MAP)        minimalSetWindowTitle(app, "Frost | Map Editor");
    else if (scene_editor.mode == SCENE_EDIT_WORLD) minimalSetWindowTitle(app, "Frost | World Editor");
    else                                            minimalSetWindowTitle(app, "Frost");

    if (FrostDebuggerOnEvent(&debugger, e)) return MINIMAL_OK;

    SceneEditorOnEvent(&scene_editor, e);
    SceneOnEvent(&scene, e);

    return MINIMAL_OK;
}

void OnUpdate(MinimalApp* app, float deltatime)
{
    // update
    glClear(GL_COLOR_BUFFER_BIT);
    if (debugger.console.focus)                     FrostDebuggerOnUpdate(&debugger, deltatime);
    else if (SceneEditorIsActive(&scene_editor))    SceneEditorOnUpdate(&scene_editor, deltatime);
    else                                            SceneOnUpdate(&scene, deltatime);

    // render
    SceneOnRender(&scene);

    // render debug
    if (app->debug) SceneOnRenderDebug(&scene);

    // render ui
    if (!SceneEditorIsActive(&scene_editor)) SceneOnRenderUI(&scene);

    SceneEditorOnRender(&scene_editor);
    FrostDebuggerOnRenderUI(&debugger, app);
}

int main()
{
    MinimalApp app = { 
        .on_load = OnLoad,
        .on_destroy = OnDestroy,
        .on_event = OnEvent,
        .on_update = OnUpdate
    };

    if (FrostLoad(&app, "config.ini"))
        minimalRun(&app);

    minimalDestroy(&app);

    size_t bytes = FrostMemoryGetBytes();
    if (bytes != 0) MINIMAL_WARN("%llu bytes not freed", bytes);

    return 0;
}