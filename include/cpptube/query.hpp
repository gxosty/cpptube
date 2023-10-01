#pragma once

#include <cpptube/streams.hpp>
#include <vector>
#include <map>

namespace cpptube::query
{
	class StreamQuery
	{
	private:
		std::vector<cpptube::streams::Stream>* fmt_streams;
		std::map<int, cpptube::streams::Stream*> itag_index;

	public:
		StreamQuery(std::vector<cpptube::streams::Stream>* fmt_streams);

		cpptube::streams::Stream* first();
	};
}