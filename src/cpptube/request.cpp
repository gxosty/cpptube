#include <cpptube/request.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/exceptions.hpp>
#include <cpptube/logger.hpp>
#include <curl/curl.h>

#include <mutex>
#include <iostream>
#include <regex>

namespace cpptube::request
{
	cpptube::logger::Logger logger(__FILE__);
	std::mutex __request_mutex;
	std::mutex __dns_mutex;

	CURL* __curl = nullptr;
	CURL* __curl_dns = nullptr;

	bool __check_status(CURL* curl, CURLcode curle_code, int status_code)
	{
		logger.info() << "CURLcode == " << (int)curle_code << std::endl;
		logger.info() << "status_code == " << status_code << std::endl;

		if ((curle_code != CURLE_OK) && (curle_code != CURLE_WRITE_ERROR))
		{
			logger.warning() << "Request wasn't successful" << std::endl;
		}
		else if (status_code >= 400)
		{
			curl_easy_reset(curl);
			throw cpptube::exceptions::HTTPStatusCodeError(status_code);
		}

		return curle_code != CURLE_OK;
	}

	size_t __write_function(void* data, size_t size, size_t nmemb, void* userp)
	{
		size_t full_size = size * nmemb;
		((std::string*)userp)->append((char*)data, full_size);
		return full_size;
	}

	size_t __headers_write_function(void* data, size_t size, size_t nmemb, void* userp)
	{
		size = size * nmemb;
		nlohmann::json* headers = (nlohmann::json*)userp;
		std::string header = std::string((char*)data, size);

		if (!header.empty())
		{
			auto found = header.find(": ");

			if (found != std::string::npos)
			{
				std::string header_value;

				if (header.find("\r\n") != std::string::npos)
				{
					header_value = header.substr(found + 2, header.size() - found - 2 - 2);
				}
				else if (header.find("\n") != std::string::npos)
				{
					header_value = header.substr(found + 2, header.size() - found - 2 - 1);
				}
				else
				{
					header_value = header.substr(found + 2, header.size() - found - 2);
				}

				(*headers)[header.substr(0, found)] = header_value;
			}
		}

		return size;
	}

	std::string __dns_over_https(const std::string& domain_name)
	{
		/**
		 * Returns IP address of associated `domain_name` using
		 * Google Public DNS
		 */

		static const std::pair<std::string, std::string> dns_resolver = {
			"google-public-dns-a.google.com", "216.239.36.36"
		};

		__dns_mutex.lock();

		logger.debug() << "Resolving IP of: " << domain_name << std::endl;

		std::string url = "https://www.google.com/resolve?name=" + domain_name;

		if (__curl_dns == nullptr)
		{
			__curl_dns = curl_easy_init();
		}

		curl_easy_reset(__curl_dns);

		std::string result;
		curl_easy_setopt(__curl_dns, CURLOPT_FOLLOWLOCATION, 1LL);
		curl_easy_setopt(__curl_dns, CURLOPT_SSL_VERIFYPEER, 0LL);
		curl_easy_setopt(__curl_dns, CURLOPT_SSL_VERIFYHOST, 0LL);
		curl_easy_setopt(__curl_dns, CURLOPT_WRITEFUNCTION, __write_function);
		curl_easy_setopt(__curl_dns, CURLOPT_WRITEDATA, (void*)&result);

		std::string resolve_string = "www.google.com:443:" + dns_resolver.second;
		logger.debug() << "Resolver: " << resolve_string << std::endl;

		struct curl_slist* _resolve_data = nullptr;
		_resolve_data = curl_slist_append(_resolve_data, resolve_string.c_str());
		if (curl_easy_setopt(__curl_dns, CURLOPT_RESOLVE, _resolve_data) != CURLE_OK)
		{
			logger.error() << "Unknown option: CURLOPT_RESOLVE" << std::endl;
			curl_easy_reset(__curl_dns);
			return "";
		}
		curl_easy_setopt(__curl_dns, CURLOPT_DNS_USE_GLOBAL_CACHE, false);

		struct curl_slist* _headers = nullptr;

		std::string host = "Host: " + dns_resolver.first;

		_headers = curl_slist_append(_headers, "accept-language: en-US,en");
		_headers = curl_slist_append(_headers, "User-Agent: Mozilla/5.0");
		_headers = curl_slist_append(_headers, host.c_str());

		curl_easy_setopt(__curl_dns, CURLOPT_URL, url.c_str());
		curl_easy_setopt(__curl_dns, CURLOPT_HTTPHEADER, _headers);

		CURLcode curle_code = CURLE_OK;
		int status_code = 0;
		int retries = 5;

		do {
			logger.debug() << "Requesting url: " << url << std::endl;
			curle_code = curl_easy_perform(__curl_dns);
			curl_easy_getinfo(__curl_dns, CURLINFO_RESPONSE_CODE, &status_code);
		} while ((__check_status(__curl_dns, curle_code, status_code)) && ((--retries) > 0));

		if (result.empty())
		{
			curl_easy_reset(__curl_dns);
			return "";
		}

		nlohmann::json json_data = nlohmann::json::parse(result);

		std::string ip = domain_name;
		bool search = true;

		while (search)
		{
			search = false;

			for (auto& item : json_data["Answer"].items())
			{
				if (item.value()["name"].get<std::string>().find(ip) != std::string::npos)
				{
					ip = item.value()["data"].get<std::string>();
					search = true;
				}
			}
		}

		curl_easy_reset(__curl_dns);
		logger.debug() << "Host -> IP: " << domain_name << " -> " << ip << std::endl;
		__dns_mutex.unlock();

		return ip;
	}

	void __domain_front(CURL* curl, std::string& url, struct curl_slist* headers)
	{
		static const std::regex redirector_pattern("^https://([a-z0-9\\-\\.]+?\\.googlevideo\\.com)");
		static const std::regex youtube_pattern("(?:[a-z]*?\\.)?youtube\\.com");

		if (url.find("googlevideo.com") != std::string::npos)
		{
			std::smatch matches;
			std::regex_search(url, matches, redirector_pattern);

			if (!matches.empty())
			{
				const std::string& domain_name = matches[1].str();

				std::string ip = __dns_over_https(domain_name);
				std::string resolve_string = domain_name + ":443:" + ip;
				struct curl_slist* _resolve_data = nullptr;
				_resolve_data = curl_slist_append(_resolve_data, resolve_string.c_str());

				// headers = curl_slist_append(headers, ("Host: " + domain_name).c_str());
				// url = std::regex_replace(url, redirector_pattern, "https://www.google.com");
				curl_easy_setopt(__curl, CURLOPT_RESOLVE, _resolve_data);
				curl_easy_setopt(__curl, CURLOPT_DNS_USE_GLOBAL_CACHE, false);
			}

		}
		else if (url.find("youtube.com"))
		{
			url = std::regex_replace(url, youtube_pattern, "www.google.com");
			headers = curl_slist_append(headers, "Host: www.youtube.com");
		}
	}

	void __execute_request(
		size_t(*write_function)(void*, size_t, size_t, void*),
		void* write_data,
		std::string url,
		CPPTUBE_REQUEST_METHOD method,
		nlohmann::json headers,
		const std::string* data,
		long timeout,
		long retries,

		nlohmann::json* response_headers
	) {
		__request_mutex.lock();
		if (__curl == nullptr)
		{
			curl_global_init(CURL_GLOBAL_ALL);
			__curl = curl_easy_init();
		}

		curl_easy_reset(__curl);

		curl_easy_setopt(__curl, CURLOPT_FOLLOWLOCATION, 1LL);

		if (write_data && write_function)
		{
			curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, write_function);
			curl_easy_setopt(__curl, CURLOPT_WRITEDATA, write_data);
		}

		switch (method)
		{
		case METHOD_GET:
			break;
		case METHOD_POST:
			curl_easy_setopt(__curl, CURLOPT_POST, 1LL);
			break;
		case METHOD_HEAD:
			curl_easy_setopt(__curl, CURLOPT_NOBODY, 1LL);
			break;
		}

		struct curl_slist* _headers = nullptr;

		_headers = curl_slist_append(_headers, "accept-language: en-US,en");
		_headers = curl_slist_append(_headers, "User-Agent: Mozilla/5.0");
		for (auto& item : headers.items())
		{
			std::string header = item.key() + ": " + item.value().get<std::string>(); 
			_headers = curl_slist_append(_headers, header.c_str());
		}

		__domain_front(__curl, url, _headers);

		curl_easy_setopt(__curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(__curl, CURLOPT_HTTPHEADER, _headers);

		if (data != nullptr)
		{
			curl_easy_setopt(__curl, CURLOPT_POSTFIELDS, data->c_str());
			curl_easy_setopt(__curl, CURLOPT_POSTFIELDSIZE, data->size());
		}

		if (response_headers != nullptr)
		{
			curl_easy_setopt(__curl, CURLOPT_HEADERFUNCTION, __headers_write_function);
			curl_easy_setopt(__curl, CURLOPT_HEADERDATA, (void*)response_headers);
		}

		curl_easy_setopt(__curl, CURLOPT_TIMEOUT, timeout);
		CURLcode curle_code = CURLE_OK;
		int status_code = 0;

		do {
			logger.debug() << "Requesting url: " << url << std::endl;
			curle_code = curl_easy_perform(__curl);
			curl_easy_getinfo(__curl, CURLINFO_RESPONSE_CODE, &status_code);
		} while ((__check_status(__curl, curle_code, status_code)) && ((--retries) > 0));

		__request_mutex.unlock();
	}

	std::string execute_request(
		std::string url,
		CPPTUBE_REQUEST_METHOD method,
		nlohmann::json headers,
		const std::string* data,
		long timeout,
		long retries,

		nlohmann::json* response_headers
	) {
		std::string response_data;
		__execute_request(__write_function, (void*)&response_data, url, method, headers, data, timeout, retries, response_headers);
		return response_data;
	}

	void stream(
		size_t(*callback)(void* buffer, size_t size_n, size_t n, void* stream_p),
		void* stream_p,
		const std::string& url,
		long timeout,
		long max_retries,
		unsigned start_byte_position
	) {
		__execute_request(
			callback,
			stream_p,
			url + (start_byte_position == 0 ? "" : "&range=" + std::to_string(start_byte_position) + "-99999999999"),
			METHOD_GET,
			{},
			nullptr,
			timeout,
			max_retries
		);
	}

	std::string get(const std::string& url, nlohmann::json headers, long timeout)
	{
		std::string response_data;
		__execute_request(__write_function, (void*)&response_data, url, METHOD_GET, headers, nullptr, timeout);
		return response_data;
	}

	nlohmann::json head(const std::string& url, nlohmann::json headers, long timeout)
	{
		nlohmann::json _headers;
		__execute_request(nullptr, nullptr, url, METHOD_HEAD, headers, nullptr, timeout, 3, &_headers);
		// logger.debug() << _headers << std::endl;
		return _headers;
	}

	int filesize(const std::string& url)
	{
		nlohmann::json headers = head(url);
		return std::stoi(headers["Content-Length"].get<std::string>());
	}
}