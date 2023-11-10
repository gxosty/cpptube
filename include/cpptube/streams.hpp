#pragma once

#include <cpptube/definitions.hpp>
#include <cpptube/monostate.hpp>
#include <nlohmann/json.hpp>

#include <string>
#include <fstream>
#include <vector>

namespace cpptube::streams
{
	class Stream
	{
	private:
		std::string __url;
		int __itag;
		std::string __mime_type;
		std::vector<std::string> __codecs;
		std::string __type;
		std::string __subtype;
		std::string __video_codec;
		std::string __audio_codec;
		bool __is_otf;
		int __bitrate;
		int __filesize; // I don't think we need more than filesize in bytes
		bool __is_dash;
		std::string __abr;
		int __fps;
		std::string __resolution;
		bool __is_3d;
		bool __is_hdr;
		bool __is_live;

		cpptube::monostate::Monostate* __stream_monostate;

		// Internal use only!
		std::ofstream* __file;
		unsigned __bytes_remaining;

	public:
		Stream(const nlohmann::json& stream, cpptube::monostate::Monostate* stream_monostate);
		bool is_adaptive();
		bool is_progressive();
		bool includes_video_track();
		bool includes_audio_track();
		std::pair<std::string, std::string> parse_codecs();
		int itag();
		std::string mime_type();
		std::vector<std::string> codecs();
		std::string type();
		std::string subtype();
		std::string video_codec();
		std::string audio_codec();
		bool is_otf();
		int bitrate();
		int filesize();
		bool is_dash();
		std::string average_bitrate();
		int fps();
		std::string resolution();
		bool is_3d();
		bool is_hdr();
		bool is_live();

		cpptube::monostate::Monostate* get_stream_monostate();

		std::string* title();

		std::string default_filename();
		std::string make_filename(std::string filename);

		fs::path download(
			std::string filename = "",
			bool skip_existing = true,
			int timeout = 0, // downloading will stop after this time even if downloading is going
			int max_retries = 0,
			bool continue_download = false
		);

		bool exists_at_path(fs::path filepath);

		void on_progress(void* ptr, size_t size);
		void on_complete(const fs::path& filepath);
	};
}