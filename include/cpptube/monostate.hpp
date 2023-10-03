#pragma once

#include <cpptube/definitions.hpp>

namespace cpptube::monostate
{
	class Monostate {
	public:
		void(*on_progress_callback)(void* stream, void* buffer, size_t buffer_size, long bytes_remaining);
		void(*on_complete_callback)(fs::path file_path);

		std::string title;
		unsigned duration;

		Monostate(
			void(*on_progress_callback)(void* stream, void* buffer, size_t buffer_size, long bytes_remaining) = nullptr,
			void(*on_complete_callback)(fs::path file_path) = nullptr
		);
	};
}