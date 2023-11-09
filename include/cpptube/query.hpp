#pragma once

#include <cpptube/streams.hpp>
#include <nlohmann/json.hpp>

#include <vector>
#include <map>
#include <functional>

namespace cpptube::query
{
	class StreamQuery
	{
	private:
		std::vector<cpptube::streams::Stream>* fmt_streams;
		std::map<int, cpptube::streams::Stream*> itag_index;

		void __filter(
			std::vector<cpptube::streams::Stream*>* list,
			std::function<bool(cpptube::streams::Stream* stream)> filter_function
		);

		std::vector<cpptube::streams::Stream*> create_list();

	public:
		StreamQuery(std::vector<cpptube::streams::Stream>* fmt_streams);

		std::vector<cpptube::streams::Stream*> filter(const nlohmann::json& filter_list);

		cpptube::streams::Stream* first();
		cpptube::streams::Stream* get_by_itag(int itag);

		size_t size();
	};
}