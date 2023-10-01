#include <cpptube/query.hpp>

namespace cpptube::query
{
	StreamQuery::StreamQuery(std::vector<cpptube::streams::Stream>* fmt_streams)
	{
		this->fmt_streams = fmt_streams;
		for (auto& stream : *(this->fmt_streams)) {
			this->itag_index[stream.itag()] = (cpptube::streams::Stream*)&stream;
		}
	}

	cpptube::streams::Stream* StreamQuery::first()
	{
		return &(*this->fmt_streams)[0];
	}
}