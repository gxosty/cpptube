#include <cpptube/cpptube.hpp>
#include <cpptube/extract.hpp>

namespace cpptube
{
	YouTube::YouTube(
		std::string url,
		void(*on_progress_callback)(cpptube::streams::Stream* stream, void* buffer, size_t buffer_size, long bytes_remaining),
		void(*on_complete_callback)(fs::path file_path)
	) {
		this->url = url;

		this->video_id = cpptube::extract::video_id(this->url);
		this->watch_url = "https://youtube.com/watch?v=" + this->video_id;

		this->stream_monostate = cpptube::monostate::Monostate(
			on_progress_callback, on_complete_callback
		);
	}

	std::vector<cpptube::streams::Stream>* YouTube::fmt_streams()
	{
		if (!this->__fmt_streams.empty())
			return &this->__fmt_streams;

		return nullptr;
	}

	cpptube::query::StreamQuery YouTube::streams()
	{
		return cpptube::query::StreamQuery(this->fmt_streams());
	}
}