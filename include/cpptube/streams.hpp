#pragma once

namespace cpptube::streams
{
	class Stream
	{
	private:
		int __itag;

	public:
		Stream() = default;
		int itag();
	};
}