#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace innertube
{
	class InnerTube
	{
	private:
		nlohmann::json __context;
		nlohmann::json __header;
		std::string __api_key;
		std::string __access_token;
		std::string __refresh_token;
		bool __use_oauth;
		bool __allow_cache;

		unsigned __expires;

		nlohmann::json __call_api(const std::string& endpoint, nlohmann::json query, const std::string& data);

	public:
		InnerTube(
			std::string client = "ANDROID",
			bool use_oauth = false,
			bool allow_cache = true
		);

		void cache_tokens();
		void refresh_bearer_token(bool force = false);
		void fetch_bearer_token();

		// properties
		std::string base_url();
		nlohmann::json base_data();
		nlohmann::json base_params();

		nlohmann::json player(const std::string& video_id);
		nlohmann::json search(const std::string& search_query, void* continuation = nullptr);
		nlohmann::json verify_age(const std::string& video_id);
		nlohmann::json get_transcript(const std::string& video_id);
	};
}