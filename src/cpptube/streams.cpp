#include <cpptube/streams.hpp>
#include <cpptube/request.hpp>
#include <cpptube/extract.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/itags.hpp>
#include <cpptube/logger.hpp>

#include <iostream>

namespace cpptube::streams
{
	cpptube::logger::Logger logger(__FILE__);

	size_t __stream_download(void* buffer, size_t size_n, size_t n, void* stream_p)
	{
		size_t size = size_n * n;
		Stream* stream = reinterpret_cast<Stream*>(stream_p);
		stream->on_progress(buffer, size, 0);
		return size;
	}

	Stream::Stream(const nlohmann::json& stream, cpptube::monostate::Monostate* stream_monostate)
		: __stream_monostate{stream_monostate} {

		// logger.debug() << "Stream JSON: " << stream << std::endl;

		this->__url = stream["url"].get<std::string>();
		this->__itag = stream["itag"].get<int>();

		// logger.debug() << "Url: " << this->__url << std::endl;
		// logger.debug() << "Itag: " << this->__itag << std::endl;
		// logger.debug() << "Url and Itag set" << std::endl;

		std::pair<std::string, std::vector<std::string>> mtc = cpptube::extract::mime_type_codec(stream["mimeType"].get<std::string>());
		this->__mime_type = mtc.first;
		this->__codecs = mtc.second;

		// logger.debug() << "MimeType and codecs set" << std::endl;

		nlohmann::json type_subtype = cpptube::helpers::split_string(this->__mime_type, "/");
		this->__type = type_subtype[0].get<std::string>();
		this->__subtype = type_subtype[1].get<std::string>();

		// logger.debug() << "Type and SubType set" << std::endl;	

		std::pair<std::string, std::string> codecs = this->parse_codecs();
		this->__video_codec = codecs.first;
		this->__audio_codec = codecs.second;

		// logger.debug() << "VideoCodec and AudioCodec set" << std::endl;

		this->__is_otf = stream["is_otf"].get<bool>();
		this->__bitrate = stream["bitrate"].get<int>();

		// logger.debug() << "`__is_otf` and Bitrate set" << std::endl;

		this->__filesize = (stream.contains("contentLength") ? (int)std::stoi(stream["contentLength"].get<std::string>()) : 0);

		// logger.debug() << "Filesize set" << std::endl;

		nlohmann::json itag_profile = cpptube::itags::get_format_profile(this->__itag);
		this->__is_dash = itag_profile["is_dash"].get<bool>();
		this->__abr = itag_profile["abr"].get<std::string>();

		// logger.debug() << "`__is_dash` and ABR set" << std::endl;

		this->__fps = (stream.contains("fps") ? stream["fps"].get<int>() : 0);

		// logger.debug() << "FPS set" << std::endl;

		this->__resolution = itag_profile["resolution"].get<std::string>();
		this->__is_3d = itag_profile["is_3d"].get<bool>();
		this->__is_hdr = itag_profile["is_hdr"].get<bool>();
		this->__is_live = itag_profile["is_live"].get<bool>();

		// logger.debug() << "Stream Ready" << std::endl;
	};

	bool Stream::is_adaptive()
	{
		return (bool)(this->__codecs.size() % 2);
	}

	bool Stream::is_progressive()
	{
		return !this->is_adaptive();
	}

	bool Stream::includes_video_track()
	{
		return (this->is_progressive() || (this->__type == "video"));
	}

	bool Stream::includes_audio_track()
	{
		return (this->is_progressive() || (this->__type == "audio"));
	}

	std::pair<std::string, std::string> Stream::parse_codecs()
	{
		std::pair<std::string, std::string> video_audio;
		// logger.debug() << "codecsize: " << (int)this->__codecs.size() << std::endl;
		// logger.debug() << "__type: " << this->__type << std::endl;

		if (!this->is_adaptive())
		{
			video_audio = {this->__codecs[0], this->__codecs[1]};
		}
		else if (this->includes_video_track())
		{
			video_audio.first = this->__codecs[0];
		}
		else if (this->includes_audio_track())
		{
			video_audio.second = this->__codecs[0];
		}

		return video_audio;
	}

	int Stream::itag()
	{
		return this->__itag;
	}

	std::string Stream::mime_type()
	{
		return this->__mime_type;
	}

	std::vector<std::string> Stream::codecs()
	{
		return this->__codecs;
	}

	std::string Stream::type()
	{
		return this->__type;
	}

	std::string Stream::subtype()
	{
		return this->__subtype;
	}

	std::string Stream::video_codec()
	{
		return this->__video_codec;
	}

	std::string Stream::audio_codec()
	{
		return this->__audio_codec;
	}

	bool Stream::is_otf()
	{
		return this->__is_otf;
	}

	int Stream::bitrate()
	{
		return this->__bitrate;
	}

	int Stream::filesize()
	{
		if (this->__filesize == 0)
		{
			this->__filesize = cpptube::request::filesize(this->__url);
		}

		return this->__filesize;
	}

	bool Stream::is_dash()
	{
		return this->__is_dash;
	}

	std::string Stream::average_bitrate()
	{
		return this->__abr;
	}

	int Stream::fps()
	{
		return this->__fps;
	}

	std::string Stream::resolution()
	{
		return this->__resolution;
	}

	bool Stream::is_3d()
	{
		return this->__is_3d;
	}

	bool Stream::is_hdr()
	{
		return this->__is_hdr;
	}

	bool Stream::is_live()
	{
		return this->__is_live;
	}

	cpptube::monostate::Monostate* Stream::get_stream_monostate()
	{
		return this->__stream_monostate;
	}

	std::string* Stream::title()
	{
		logger.debug() << "Title: " << this->__stream_monostate->title << std::endl;
		return &(this->__stream_monostate->title);
	}

	std::string Stream::default_filename()
	{
		std::string filename = cpptube::helpers::safe_filename(*this->title());

		return filename + "." + this->__subtype;
	}

	std::string Stream::make_filename(std::string filename)
	{
		if (filename.empty())
		{
			filename = this->default_filename();
		}

		return filename;
	}

	fs::path Stream::download(
		std::string filename,
		bool skip_existing,
		int timeout,
		int max_retries,
		bool continue_download
	) {
		fs::path filepath(filename);
		filepath = filepath.parent_path() / fs::path(this->make_filename(filepath.filename().string()));

		if (skip_existing && this->exists_at_path(filepath))
		{
			logger.debug() << "File \"" << filepath.string() << "\" already exists, skipping" << std::endl;
			this->on_complete(filepath);
			return filepath;
		}

		logger.debug() << "Downloading file to: " << filepath.string() << std::endl;

		this->__file = new std::ofstream(filepath);

		if (!this->__file->is_open())
		{
			throw std::runtime_error("Couldn't open a file: " + filename);
		}

		this->__bytes_remaining = this->filesize();
		cpptube::request::__execute_request(
			__stream_download,
			(void*)this,
			this->__url,
			METHOD_GET,
			{},
			nullptr,
			timeout,
			max_retries
		);

		this->__file->close();
		delete this->__file;
		this->on_complete(filepath);

		return filepath;
	}

	bool Stream::exists_at_path(fs::path filepath)
	{
		return fs::exists(filepath) && ((int)fs::file_size(filepath) == this->filesize());
	}

	void Stream::on_progress(void* ptr, size_t size, long bytes_remaining)
	{

	}

	void Stream::on_complete(fs::path filepath)
	{

	}
}