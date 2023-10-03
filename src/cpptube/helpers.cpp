#include <cpptube/helpers.hpp>
#include <regex>
#include <curl/curl.h>

namespace cpptube::helpers
{
	std::string regex_search(const std::string& pattern, const std::string& str, int group)
	{
		std::regex regex_pattern(pattern);
		std::smatch matches;

		std::regex_search(str, matches, regex_pattern);
		return matches[group].str();
	}

	std::string url_encode(const std::string& url)
	{
		// char* encoded_url = curl_escape(url.c_str(), url.size());
		// TODO: throw exception if `encoded_url` is :nullptr:
		// return std::string(encoded_url);

		// seems like it doesn't work how we wanted...
		return url;
	}

	std::string url_encode(const nlohmann::json& query)
	{
		std::string query_str = "";

		for (auto& item : query.items())
		{
			if (!query_str.empty())
			{
				query_str += "&";
			}
			query_str += item.key() + "=";

			if (item.value().is_string())
			{
				query_str += item.value().get<std::string>();
			}
			else
			{
				query_str += item.value().dump();
			}
		}

		return url_encode(query_str);
	}
}