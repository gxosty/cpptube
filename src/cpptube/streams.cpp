#include <cpptube/streams.hpp>

namespace cpptube::streams
{
	Stream::Stream(const nlohmann::json& stream, cpptube::monostate::Monostate* stream_monostate)
		: __stream{stream}, __stream_monostate{stream_monostate} {

	};

	int Stream::itag()
	{
		return this->__itag;
	}
}