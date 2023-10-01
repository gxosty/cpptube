#include <cpptube/helpers.hpp>
#include <regex>

namespace cpptube::helpers
{
	std::string regex_search(const std::string& pattern, const std::string& str, int group)
	{
		std::regex regex_pattern(pattern);
		std::smatch matches;

		std::regex_search(str, matches, regex_pattern);
		return matches[group].str();
	}
}