#include <cpptube/query.hpp>
#include <cpptube/logger.hpp>

namespace cpptube::query
{
	cpptube::logger::Logger logger(__FILE__);

	StreamQuery::StreamQuery(std::vector<cpptube::streams::Stream>* fmt_streams)
	{
		this->fmt_streams = fmt_streams;
		for (auto& stream : *(this->fmt_streams)) {
			this->itag_index[stream.itag()] = (cpptube::streams::Stream*)&stream;
		}
	}

	void StreamQuery::__filter(std::vector<cpptube::streams::Stream*>* list, std::function<bool(cpptube::streams::Stream* stream)> filter_function)
	{
		for (auto i = list->end() - 1; i >= list->begin(); i--)
		{
			if (!filter_function(*i))
			{
				list->erase(i);
			}
		}
	}

	std::vector<cpptube::streams::Stream*> StreamQuery::create_list()
	{
		std::vector<cpptube::streams::Stream*> streams{};

		for (auto& stream : *(this->fmt_streams))
		{
			streams.push_back(&stream);
		}

		return streams;
	}

	std::vector<cpptube::streams::Stream*> StreamQuery::filter(const nlohmann::json& filter_list)
	{
		auto streams_list = this->create_list();
		std::string temp_key = "";

		#define FILTER_CALLBACK(cb, ...) std::function<bool(cpptube::streams::Stream* stream)> cb = [__VA_ARGS__](cpptube::streams::Stream* stream) -> bool

		if (filter_list.contains("fps"))
		{
			int fps = filter_list["fps"].get<int>();
			FILTER_CALLBACK(callback, &fps) {
				return stream->fps() == fps;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("resolution"))
		{
			temp_key = "resolution";
		}
		else
		{
			temp_key = "res";
		}

		if (filter_list.contains(temp_key))
		{
			std::vector<std::string> res;

			if (filter_list[temp_key].is_string())
			{
				res.push_back(filter_list[temp_key].get<std::string>());
			}
			else if (filter_list[temp_key].size() > 1) /* is_vector */
			{
				res = filter_list[temp_key].get<std::vector<std::string>>();
			}

			if (!res.empty())
			{
				FILTER_CALLBACK(callback, &res) {
					for (auto& r : res)
					{
						if (stream->resolution() == r)
						{
							return true;
						}
					}

					return false;
				};
				__filter(&streams_list, callback);
			}
		}

		if (filter_list.contains("mime_type"))
		{
			std::string mime_type = filter_list["mime_type"].get<std::string>();

			FILTER_CALLBACK(callback, &mime_type) {
				return stream->mime_type() == mime_type;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("type"))
		{
			std::string type = filter_list["type"].get<std::string>();

			FILTER_CALLBACK(callback, &type) {
				return stream->type() == type;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("file_extension"))
		{
			temp_key = "file_extension";
		}
		else
		{
			temp_key = "subtype";
		}

		if (filter_list.contains(temp_key))
		{
			std::string subtype = filter_list[temp_key].get<std::string>();

			FILTER_CALLBACK(callback, &subtype) {
				return stream->subtype() == subtype;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("abr"))
		{
			temp_key = "abr";
		}
		else
		{
			temp_key = "bitrate";
		}

		if (filter_list.contains(temp_key))
		{
			int bitrate = filter_list[temp_key].get<int>();

			FILTER_CALLBACK(callback, &bitrate) {
				return stream->bitrate() == bitrate;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("video_codec"))
		{
			std::string video_codec = filter_list["video_codec"].get<std::string>();

			FILTER_CALLBACK(callback, &video_codec) {
				return stream->video_codec() == video_codec;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("audio_codec"))
		{
			std::string audio_codec = filter_list["audio_codec"].get<std::string>();

			FILTER_CALLBACK(callback, &audio_codec) {
				return stream->audio_codec() == audio_codec;
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("only_video"))
		{
			FILTER_CALLBACK(callback) {
				return stream->includes_video_track() && (!stream->includes_audio_track());
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("only_audio"))
		{
			FILTER_CALLBACK(callback) {
				return (!stream->includes_video_track()) && stream->includes_audio_track();
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("progressive"))
		{
			FILTER_CALLBACK(callback) {
				return stream->is_progressive();
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("adaptive"))
		{
			FILTER_CALLBACK(callback) {
				return stream->is_adaptive();
			};
			__filter(&streams_list, callback);
		}

		if (filter_list.contains("is_dash"))
		{
			bool is_dash = filter_list["is_dash"].get<bool>();
			FILTER_CALLBACK(callback, &is_dash) {
				return stream->is_dash() == is_dash;
			};
			__filter(&streams_list, callback);
		}

		return streams_list;
	}

	cpptube::streams::Stream* StreamQuery::first()
	{
		return &(*this->fmt_streams)[0];
	}

	size_t StreamQuery::size()
	{
		return this->fmt_streams->size();
	}
}