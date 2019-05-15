#pragma once

#include <Windows.h>

bool CheckStorage(const DWORDLONG diskSpaceNeeded);
DWORD ReadCPUSpeed();
bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);