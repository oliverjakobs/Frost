#pragma once

#include "Log/Logger.h"

#include <iostream>
#include <algorithm>

#include <map>

#include <memory>
#include <utility>

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;

#ifdef _DEBUG
#define DEBUG_ASSERT(x, ...) { if(!(x)) { DEBUG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#define DEBUG_ASSERT(x, ...)

#define DEBUG_NEW new
#endif 