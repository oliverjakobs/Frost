#pragma once

#include <iostream>

void* operator new(size_t size);
void operator delete(void* memory);