#pragma once

#include <iostream>

#ifdef _DEBUG
#define DEBUG_MESSAGE(s) {std::clog << s << "\n";}
#else
#define DEBUG_MESSAGE(s) {std::clog << s << "\n";}
#endif

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;