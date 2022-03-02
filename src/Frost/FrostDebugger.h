#ifndef FROST_DEBUGGER_H
#define FROST_DEBUGGER_H

#include "Frost.h"

#include "Debug/Console.h"

typedef struct
{
    int show_info;
    Console console;
    
    Scene* scene;
    SceneEditor* editor;
} FrostDebugger;

void FrostDebuggerInit(FrostDebugger* debugger, int show_info, IgnisFont* font);
void FrostDebuggerBindScene(FrostDebugger* debugger, Scene* scene, SceneEditor* editor);

void ForstDebuggerShowInfo(FrostDebugger* debugger, int b);
void FrostDebuggerToggleInfo(FrostDebugger* debugger);

int FrostDebuggerOnEvent(FrostDebugger* debugger, const MinimalEvent* e);
void FrostDebuggerOnUpdate(FrostDebugger* debugger, float deltatime);
void FrostDebuggerOnRenderUI(FrostDebugger* debugger, const MinimalApp* app);

void FrostExecuteConsoleCommand(Console* console, FrostDebugger* debugger, const char* cmd_buffer);

#endif /* !FROST_DEBUGGER_H */
