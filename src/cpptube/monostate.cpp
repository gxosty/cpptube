#include <cpptube/monostate.hpp>

namespace cpptube::monostate
{
	Monostate::Monostate(
		void(*on_progress_callback)(void* stream, void* buffer, size_t buffer_size, long bytes_remaining),
		void(*on_complete_callback)(fs::path file_path)
	) {
		this->on_progress_callback = on_progress_callback;
		this->on_complete_callback = on_complete_callback;
	}
}