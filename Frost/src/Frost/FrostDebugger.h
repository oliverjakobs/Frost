#ifndef FROST_DEBUGGER_H
#define FROST_DEBUGGER_H

#include "Frost.h"

typedef struct
{
    int show;
} FrostDebugger;

void FrostDebuggerInit(FrostDebugger* debugger);

void ForstDebuggerShow(FrostDebugger* debugger, int b);
void FrostDebuggerToggleDisplay(FrostDebugger* debugger);

void FrostDebugRenderSettings(const FrostDebugger* debugger, float x, float y);
void FrostDebugRenderInfo(const FrostDebugger* debugger, const Scene* scene, float x, float y);

#endif /* !FROST_DEBUGGER_H */
