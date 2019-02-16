#pragma once

#include <iostream>

#define DEBUG_MESSAGE(s) {std::clog << s << "\n";}

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;