#pragma once

#include <string>

typedef void(*)(cpptube::streams::Stream*, void* buffer, size_t buffer_size, long bytes_remaining) CPPTUBE_ON_PROGRESS_CALLBACK;

namespace cpptube {
	class YouTube {
	private:

	public:
		YouTube(
			std::string url,

		);
	}
}