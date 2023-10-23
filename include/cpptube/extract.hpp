#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace cpptube::extract
{
	std::string video_id(const std::string& url);
	std::string js_url(const std::string* url);
	std::pair<std::string, std::vector<std::string>> mime_type_codec(const std::string& mtc);
	nlohmann::json apply_descrambler(nlohmann::json* stream_data);
	void apply_signature(nlohmann::json* stream_manifest, nlohmann::json* vid_info, std::string* js);
}