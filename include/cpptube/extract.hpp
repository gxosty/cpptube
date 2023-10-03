#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace cpptube::extract
{
	std::string video_id(const std::string& url);
	std::string js_url(const std::string* watch_url);
	nlohmann::json apply_descrambler(nlohmann::json* stream_data);
	void apply_signature(nlohmann::json* stream_manifest, nlohmann::json* vid_info, std::string* js);
}