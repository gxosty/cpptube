#pragma once

#include <nlohmann/json.hpp>

#include <unordered_map>
#include <string>

namespace cpptube::itags
{
	extern std::unordered_map<int, std::pair<std::string, std::string>> PROGRESSIVE_VIDEO;
	extern std::unordered_map<int, std::pair<std::string, std::string>> DASH_VIDEO;
	extern std::unordered_map<int, std::pair<std::string, std::string>> DASH_AUDIO;
	extern std::vector<int> HDR;
	extern std::vector<int> _3D;
	extern std::vector<int> LIVE;

	extern nlohmann::json get_format_profile(const int& itag);
}