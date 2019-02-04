#pragma once

#include <string>
#include <vector>

#include <sstream>

namespace sb
{
	inline bool stringCompare(const std::string& s1, const std::string& s2)
	{
		return s1.compare(s2) == 0;
	}

	inline std::vector<std::string> cutString(const std::string& c, const std::string& string)
	{
		std::string rest = string;
		std::vector<std::string> lines;

		for (std::size_t pos = rest.find(c); pos != std::string::npos; pos = rest.find(c))
		{
			std::string line = rest.substr(0, pos);
			rest = rest.substr(pos + c.length());

			if (!line.empty())
				lines.push_back(line);
		}

		if (!rest.empty())
			lines.push_back(rest);

		return lines;
	}

	inline std::vector<std::string> cutStringBefore(const std::string& c, const std::string& string)
	{
		std::string rest = string;
		std::vector<std::string> lines;

		for (std::size_t pos = rest.find(c); pos != std::string::npos; pos = rest.find(c, c.length()))
		{
			std::string line = rest.substr(0, pos);
			rest = rest.substr(pos);

			if (!line.empty())
				lines.push_back(line);
		}

		if (!rest.empty())
			lines.push_back(rest);

		return lines;
	}

	inline std::vector<std::size_t> findStrings(const std::string& str, std::vector<std::string> strings)
	{
		std::vector<std::size_t> pos;

		for (auto s : strings)
		{
			pos.push_back(str.find(s));
		}

		return pos;
	}

	inline std::string getValue(const std::string& name, const std::string& string)
	{
		std::size_t pos = string.find(name);

		if (pos != std::string::npos)
		{
			return string.substr(pos + name.length());
		}

		return std::string();
	}

	template <typename T>
	std::string toString(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}

	template< typename... argv >
	std::string stringf(const char* format, argv... args) 
	{
		const size_t SIZE = std::snprintf(NULL, 0, format, args...);

		std::string output;
		output.resize(SIZE + 1);
		std::snprintf(&(output[0]), SIZE + 1, format, args...);
		return std::move(output);
	}
}