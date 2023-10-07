#include <cpptube/request.hpp>
#include <cpptube/logger.hpp>
#include <curl/curl.h>

#include <mutex>
#include <iostream>
#include <regex>

namespace cpptube::request
{
	cpptube::logger::Logger logger(__FILE__);
	std::mutex __request_mutex;

	CURL* __curl = nullptr;

	void __domain_front(CURL* curl, std::string& url, struct curl_slist* headers)
	{
		static const std::regex redirector_pattern("^https://([a-z0-9\\-\\.]+?\\.googlevideo\\.com)(/.+)");
		static const std::regex youtube_pattern("(?:[a-z]*?\\.)?youtube\\.com");

		if (url.find("googlevideo.com") != std::string::npos)
		{
			std::smatch matches;
			std::regex_search(url, matches, redirector_pattern);

			if (!matches.empty())
			{
				headers = curl_slist_append(headers, ("Host: " + matches[0].str()).c_str());
				url = std::regex_replace(url, redirector_pattern, "www.google.com");
			}

		}
		else if (url.find("youtube.com"))
		{
			url = std::regex_replace(url, youtube_pattern, "www.google.com");
			headers = curl_slist_append(headers, "Host: www.youtube.com");
		}
	}

	bool __check_status(CURLcode curle_code, int status_code)
	{
		if ((curle_code != CURLE_OK) || ((status_code < 200) || (status_code >= 300)))
		{
			logger.warning() << "Request wasn't successful" << std::endl;
			logger.warning() << "CURLcode == " << (int)curle_code << std::endl;
			logger.warning() << "status_code == " << status_code << std::endl;
		}
		return curle_code != CURLE_OK;
	}

	size_t __write_function(void* data, size_t size, size_t nmemb, void* userp)
	{
		size_t full_size = size * nmemb;
		((std::string*)userp)->append((char*)data, full_size);
		return full_size;
	}

	void __execute_request(
		size_t(*write_function)(void*, size_t, size_t, void*),
		void* write_data,
		std::string url,
		CPPTUBE_REQUEST_METHOD method,
		nlohmann::json headers,
		const std::string* data,
		long timeout,
		long retries
	) {
		__request_mutex.lock();
		if (__curl == nullptr)
		{
			curl_global_init(CURL_GLOBAL_ALL);
			__curl = curl_easy_init();
		}

		curl_easy_reset(__curl);

		curl_easy_setopt(__curl, CURLOPT_FOLLOWLOCATION, 1LL);
		curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(__curl, CURLOPT_WRITEDATA, write_data);

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

		curl_easy_setopt(__curl, CURLOPT_TIMEOUT, timeout);
		CURLcode curle_code = CURLE_OK;
		int status_code = 0;

		do {
			logger.debug() << "Requesting url: " << url << std::endl;
			curle_code = curl_easy_perform(__curl);
			curl_easy_getinfo(__curl, CURLINFO_RESPONSE_CODE, &status_code);
		} while ((__check_status(curle_code, status_code)) && ((--retries) > 0));

		__request_mutex.unlock();
	}

	std::string execute_request(
		std::string url,
		CPPTUBE_REQUEST_METHOD method,
		nlohmann::json headers,
		const std::string* data,
		long timeout,
		long retries
	) {
		std::string response_data;
		__execute_request(__write_function, (void*)&response_data, url, method, headers, data, timeout, retries);
		return response_data;
	}

	std::string get(const std::string& url, nlohmann::json headers, long timeout)
	{
		std::string response_data;
		__execute_request(__write_function, (void*)&response_data, url, METHOD_GET, headers, nullptr, timeout);
		return response_data;
	}
}