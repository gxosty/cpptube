#pragma once

#include <cpptube/definitions.hpp>
#include <cpptube/streams.hpp>

namespace cpptube::monostate
{
	class Monostate {
	public:
		void(*on_progress_callback)(cpptube::streams::Stream* stream, void* buffer, size_t buffer_size, long bytes_remaining);
		void(*on_complete_callback)(fs::path file_path);

		Monostate(
			void(*on_progress_callback)(cpptube::streams::Stream* stream, void* buffer, size_t buffer_size, long bytes_remaining) = nullptr,
			void(*on_complete_callback)(fs::path file_path) = nullptr
		);
	};
}