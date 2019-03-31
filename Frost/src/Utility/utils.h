#pragma once

#include "String/StringUtils.h"
#include "fileutils.h"

#include <iostream>
#include <algorithm>

#include <map>

#include <memory>

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;