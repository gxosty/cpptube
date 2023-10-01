#pragma once

#include <string>

namespace cpptube::helpers
{
	std::string regex_search(const std::string& pattern, const std::string& str, int group = 0);
}