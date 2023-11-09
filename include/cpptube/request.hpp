#pragma once

#include <nlohmann/json.hpp>
#include <string>

enum CPPTUBE_REQUEST_METHOD
{
	METHOD_GET,
	METHOD_POST,
	METHOD_HEAD // and so on
};

namespace cpptube::request
{	
	void __execute_request(
		// Mostly for internal use
		size_t(*write_function)(void*, size_t, size_t, void*),
		void* write_data,
		std::string url,
		CPPTUBE_REQUEST_METHOD method = METHOD_GET,
		nlohmann::json headers = {},
		const std::string* data = nullptr,
		long timeout = 10,
		long retries = 3,

		std::string* response_headers = nullptr
	);

	std::string execute_request(
		std::string url,
		CPPTUBE_REQUEST_METHOD method = METHOD_GET,
		nlohmann::json headers = {},
		const std::string* data = nullptr,
		long timeout = 10,
		long retries = 3,

		std::string* response_headers = nullptr
	);

	std::string get(const std::string& url, nlohmann::json headers = {}, long timeout = 10);

	int filesize(const std::string& url);
}