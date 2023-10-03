#pragma once

#include <cpptube/monostate.hpp>
#include <nlohmann/json.hpp>

namespace cpptube::streams
{
	class Stream
	{
	private:
		int __itag;
		nlohmann::json __stream;
		cpptube::monostate::Monostate* __stream_monostate;

	public:
		Stream(const nlohmann::json& stream, cpptube::monostate::Monostate* stream_monostate);
		int itag();
	};
}