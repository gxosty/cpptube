#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <regex>
#include <vector>

namespace cpptube::helpers
{
	std::string regex_search(const std::string& pattern, const std::string& str, int group = 0);
	std::vector<std::smatch> regex_search_all(const std::string& pattern, const std::string& str);
	std::string url_encode(const std::string& url);
	std::string url_encode(const nlohmann::json& query); // returns url encoded `query`
	nlohmann::json parse_qs(const std::string& qs);

	nlohmann::json split_string(std::string str, std::string del = " ", int count = -1);
	std::string strip_string(const std::string& str, const std::string& strip = " ");
	std::string escape_for_regex(const std::string& str);
}