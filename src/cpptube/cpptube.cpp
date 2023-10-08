#include <cpptube/cpptube.hpp>
#include <cpptube/request.hpp>
#include <cpptube/extract.hpp>
#include <cpptube/innertube.hpp>
#include <cpptube/exceptions.hpp>

typedef void(*cpptube_on_progress_callback_t)(void*, void*, size_t, long);
typedef void(*cpptube_on_complete_callback_t)(fs::path file_path);

namespace cpptube
{
	std::string __js__ = "";
	std::string __js_url__ = "";

	void set_logger_level(unsigned level)
	{
		cpptube::logger::Logger::set_level(level);
	}

	YouTube::YouTube(
		std::string url,
		void(*on_progress_callback)(cpptube::streams::Stream* stream, void* buffer, size_t buffer_size, long bytes_remaining),
		void(*on_complete_callback)(fs::path file_path)
	) {
		this->url = url;

		this->video_id = cpptube::extract::video_id(this->url);
		this->watch_url = "https://youtube.com/watch?v=" + this->video_id;

		this->stream_monostate = cpptube::monostate::Monostate(
			(cpptube_on_progress_callback_t)on_progress_callback,
			(cpptube_on_complete_callback_t)on_complete_callback
		);

		this->__js.clear();
		this->__js_url.clear();
	}

	std::string* YouTube::watch_html()
	{
		if (!this->__watch_html.empty())
			return &this->__watch_html;

		this->__watch_html = cpptube::request::get(this->watch_url);
		return &this->__watch_html;
	}

	std::string* YouTube::js_url()
	{
		if (!this->__js_url.empty())
		{
			return &this->__js_url;
		}

		if (this->age_restricted())
		{
			// this->__js_url = cpptube::extract::js_url(this->embed_html);
		}
		else
		{
			this->__js_url = cpptube::extract::js_url(this->watch_html());
		}

		return &this->__js_url;
	}

	std::string* YouTube::js()
	{
		if (!this->__js.empty())
		{
			return &this->__js;
		}

		if (cpptube::__js_url__ != *(this->js_url()))
		{
			this->__js = cpptube::request::get(*this->js_url());
			cpptube::__js__ = this->__js;
			cpptube::__js_url__ = this->__js_url;
		}
		else
		{
			this->__js = cpptube::__js__;
		}

		return &this->__js;
	}

	nlohmann::json* YouTube::vid_info()
	{
		if (!this->__vid_info.empty())
			return &this->__vid_info;

		innertube::InnerTube it("ANDROID", this->__use_oauth, this->__allow_oauth_cache);
		this->__vid_info = it.player(this->video_id);
		return &this->__vid_info;
	}

	nlohmann::json* YouTube::streaming_data()
	{
		if (!this->vid_info()->contains("streamingData"))
		{
			this->bypass_age_gate();
		}
		return &this->vid_info()[0]["streamingData"];
	}

	std::vector<cpptube::streams::Stream>* YouTube::fmt_streams()
	{
		if (!this->__fmt_streams.empty())
			return &this->__fmt_streams;

		nlohmann::json stream_manifest = cpptube::extract::apply_descrambler(this->streaming_data());

		// try {
			cpptube::extract::apply_signature(&stream_manifest, this->vid_info(), this->js());
		// } catch (cpptube::exceptions::ExtractError&) {
		// 	this->__js.clear();
		// 	this->__js_url.clear();
		// 	cpptube::__js__.clear();
		// 	cpptube::__js_url__.clear();
		// 	cpptube::extract::apply_signature(&stream_manifest, this->vid_info(), this->js());
		// }

		for (auto& stream : stream_manifest.items())
		{
			cpptube::streams::Stream video(
				stream.value(),
				&this->stream_monostate
			);
			this->__fmt_streams.push_back(video);
		}

		this->stream_monostate.title = *this->title();
		this->stream_monostate.duration = this->length();

		return &this->__fmt_streams;
	}

	cpptube::query::StreamQuery YouTube::streams()
	{
		return cpptube::query::StreamQuery(this->fmt_streams());
	}

	bool YouTube::age_restricted() {
		return this->__age_restricted;
	}

	void YouTube::bypass_age_gate()
	{
		return;
	}

	std::string* YouTube::title()
	{
		if (!this->__title.empty())
			return &this->__title;

		try {
			this->__title = this->vid_info()[0]["videoDetails"]["title"].get<std::string>();
		} catch (...) {
			throw cpptube::exceptions::CpptubeError("exception occured while accessing title of " + this->watch_url);
		}

		return &this->__title;
	}

	unsigned YouTube::length()
	{
		try {
			return (unsigned)std::stoi(this->vid_info()[0]["videoDetails"]["lengthSeconds"].get<std::string>());
		} catch (...) {}

		return 0;
	}
}