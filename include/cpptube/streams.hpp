#pragma once

#include <cpptube/monostate.hpp>
#include <nlohmann/json.hpp>

#include <string>
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
	};
}