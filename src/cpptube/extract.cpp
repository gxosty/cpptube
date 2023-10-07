#include <cpptube/extract.hpp>
#include <cpptube/parser.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/exceptions.hpp>
#include <cpptube/logger.hpp>

#include <iostream>
#include <regex>
#include <algorithm>

namespace cpptube::extract
{
	cpptube::logger::Logger logger(__FILE__);

	std::string video_id(const std::string& url)
	{
		return cpptube::helpers::regex_search("(?:v=|\\/)([0-9A-Za-z_-]{11}).*", url, 1);
	}

	nlohmann::json get_ytplayer_config(const std::string* html)
	{
		logger.debug() << "Finding initial function name" << std::endl;

		static constexpr int config_patterns_size = 2;
		std::string config_patterns[config_patterns_size] = {
			"ytplayer\\.config\\s*=\\s*",
			"ytInitialPlayerResponse\\s*=\\s*"
		};

		for (int i = 0; i < config_patterns_size; i++)
		{
			try {
				return cpptube::parser::parse_for_object(html, config_patterns + i);
			} catch (cpptube::exceptions::HTMLParseError ex) {
				logger.warning() << "Pattern failed: " << config_patterns[i] << std::endl;
				logger.warning() << ex.what() << std::endl;
				continue;
			}
		}

		static constexpr int setconfig_patterns_size = 1;
		std::string setconfig_patterns[setconfig_patterns_size] = {
			"yt\\.setConfig\\(.*['\"]PLAYER_CONFIG['\"]:\\s*"
		};

		for (int i = 0; i < setconfig_patterns_size; i++)
		{
			try {
				return cpptube::parser::parse_for_object(html, setconfig_patterns + i);
			} catch (cpptube::exceptions::HTMLParseError ex) {
				continue;
			}
		}

		throw cpptube::exceptions::RegexMatchError(
			"get_ytplayer_config", "config_patterns, setconfig_patterns"
		);
	}

	std::string get_ytplayer_js(const std::string* html)
	{
		static constexpr int js_url_patterns_size = 1;
		std::string js_url_patterns[js_url_patterns_size] = {
			"(/s/player/[\\w\\d]+/[\\w\\d_/.]+/base\\.js)"
		};

		for (int i = 0; i < js_url_patterns_size; i++)
		{
			std::regex pattern(js_url_patterns[i]);
			std::smatch matches;
			std::regex_search(*html, matches, pattern);
			if (!matches.empty())
			{
				logger.debug() << "Finished regex search, matched: " << js_url_patterns[i] << std::endl;
				return matches[1].str();
			}
		}

		throw cpptube::exceptions::RegexMatchError(
			"get_ytplayer_js", "js_url_patterns"
		);
	}

	std::string js_url(const std::string* html)
	{
		std::string base_js;

		try {
			base_js = get_ytplayer_config(html)["assets"]["js"].get<std::string>();
		} catch (...) {
			base_js = get_ytplayer_js(html);
		}
		return "https://www.youtube.com" + base_js;
	}

	nlohmann::json apply_descrambler(nlohmann::json* stream_data)
	{
		logger.debug() << "Applying descrambler" << std::endl;
		nlohmann::json formats{};
		if (stream_data->contains("formats"))
		{
			auto& stream_data__formats = stream_data->at("formats");
			for (auto& item : stream_data__formats.items())
			{
				formats += item.value();
			}
		}
		if (stream_data->contains("adaptiveFormats"))
		{
			auto& stream_data__adaptiveFormats = stream_data->at("adaptiveFormats");
			for (auto& item : stream_data__adaptiveFormats.items())
			{
				formats += item.value();
			}
		}

		for (auto& data : formats.items())
		{
			if (!data.value().contains("url"))
			{
				if (data.value().contains("signatureCipher"))
				{
					nlohmann::json cipher_url = cpptube::helpers::parse_qs(data.value()["signatureCipher"]);
					data.value()["url"] = cipher_url["url"][0].get<std::string>();
					data.value()["s"] = cipher_url["s"][0].get<std::string>();
				}
			}
			try {
				data.value()["is_otf"] = (bool)(data.value()["type"].get<std::string>() == "FORMAT_STREAM_TYPE_OTF");
			} catch (...) {
				data.value()["is_otf"] = false;
			}
		}
		return formats;
	}

	void apply_signature(nlohmann::json* stream_manifest, nlohmann::json* vid_info, std::string* js)
	{
		logger.debug() << "Applying signature" << std::endl;

		cpptube::cipher::Cipher cipher(js);

		int i = 0;
		for (auto& stream : stream_manifest->items())
		{
			std::string url;
			try {
				url = stream.value()["url"].get<std::string>();
			} catch (...) {
				bool is_live_stream = !((*vid_info)["playabilityStatus"]["liveStreamability"].empty());
				if (is_live_stream)
				{
					throw cpptube::exceptions::LiveStreamError("UNKNOWN");
				}
				throw cpptube::exceptions::CpptubeError("Couldn't get stream url");
			}

			if ((url.find("signature") != std::string::npos)
				|| (!stream.value().contains("s")
					&& ((url.find("&sig=") != std::string::npos) || (url.find("&lsig=") != std::string::npos))
					)
				)
			{
				logger.debug() << "Signature found, skip decipher" << std::endl;
				continue;
			}

			std::string signature = cipher.get_signature(stream.value()["s"].get<std::string>());

			logger.debug() << "Finished descrambling signature for itag=" << stream.value()["itag"].get<int>() << std::endl;

			nlohmann::json _query_params = cpptube::helpers::parse_qs(url);
			nlohmann::json query_params = {};

			for (auto& item : _query_params.items())
			{
				query_params[item.key()] = item.value()[0].get<std::string>();
			}

			query_params["sig"] = signature

			if (!query_params.contains("ratebypass"))
			{
				std::string initial_n = quest_params["n"].get<std::string>();
				std::string new_n = cipher.calculate_n(initial_n);
				quest_params["n"] = new_n;
			}

			url.erase(std::find(url.begin(), url.end(), '?') + 1, url.end());
			url = url + cpptube::helpers::url_encode(query_params);
		}
	}
}