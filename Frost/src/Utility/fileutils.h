#pragma once

#include <string>
#include <fstream>

inline std::string readFile(const char* path)
{
	if (path == nullptr)
		return "";

	if (*path == 0)
		return "";

	std::ifstream ifs(path);

	std::string content(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());

	return content;
}