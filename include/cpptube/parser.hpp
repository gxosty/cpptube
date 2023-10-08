#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace cpptube::parser
{
	std::string find_object_from_startpoint(const std::string* html, unsigned start_point);

	nlohmann::json parse_for_object_from_startpoint(const std::string* html, unsigned start_point);
	nlohmann::json parse_for_object(const std::string* html, std::string* pattern);

	nlohmann::json throttling_array_split(const std::string& js_array);
}