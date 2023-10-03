#pragma once

#include <cpptube/definitions.hpp>
#include <cpptube/streams.hpp>
#include <cpptube/monostate.hpp>
#include <cpptube/query.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace cpptube
{
	extern std::string __js__;
	extern std::string __js_url__;

	class YouTube
	{
	private:
		std::string url;

		bool __age_restricted;

		std::string __js;
		std::string __js_url;
		nlohmann::json __vid_info;
		std::string __watch_html;

		std::string __author;
		std::string __title;
		// TODO: public_date;

		std::vector<cpptube::streams::Stream> __fmt_streams;

		bool __use_oauth;
		bool __allow_oauth_cache;

	public:
		std::string video_id;
		std::string watch_url;

		cpptube::monostate::Monostate stream_monostate;

		YouTube(
			std::string url,
			void(*on_progress_callback)(cpptube::streams::Stream*, void* buffer, size_t buffer_size, long bytes_remaining) = nullptr,
			void(*on_complete_callback)(fs::path file_path) = nullptr
			// TODO: Implement other args
		);

		std::string* watch_html();
		std::string* js_url();
		std::string* js();
		nlohmann::json* vid_info();
		nlohmann::json* streaming_data();
		std::vector<cpptube::streams::Stream>* fmt_streams();
		cpptube::query::StreamQuery streams();
		bool age_restricted();
		void bypass_age_gate();

		std::string* title();
		unsigned length();
	};
}