#pragma once

#include <string>
#include <fstream>

inline std::string ReadFile(const char* path)
{
	if (path == nullptr || *path == 0)
		return "";

	std::ifstream ifs(path);

	std::string content(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());

	return content;
}