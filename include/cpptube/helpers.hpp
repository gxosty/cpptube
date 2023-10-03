#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace cpptube::helpers
{
	std::string regex_search(const std::string& pattern, const std::string& str, int group = 0);
	std::string url_encode(const std::string& url);
	std::string url_encode(const nlohmann::json& query); // returns url encoded `query`
}