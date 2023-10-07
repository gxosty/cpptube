#include <cpptube/helpers.hpp>
#include <cpptube/exceptions.hpp>
#include <regex>
#include <iostream>
#include <curl/curl.h>

namespace cpptube::helpers
{
	std::string regex_search(const std::string& pattern, const std::string& str, int group)
	{
		std::regex regex_pattern(pattern);
		std::smatch matches;

		std::regex_search(str, matches, regex_pattern);

		if (matches.empty())
			throw cpptube::exceptions::RegexMatchError("helpers", pattern);

		return matches[group].str();
	}

	std::string url_encode(const std::string& str)
	{
		char* encoded_str = curl_escape(str.c_str(), str.size());
		// TODO: throw exception if `encoded_url` is :nullptr:
		std::string estr = std::string(encoded_str);
		free(encoded_str);
		return estr
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
			query_str += url_encode(item.key()) + "=";

			if (item.value().is_string())
			{
				query_str += url_encode(item.value().get<std::string>());
			}
			else
			{
				query_str += url_encode(item.value().dump());
			}
		}

		return query_str;
	}

	nlohmann::json parse_qs(const std::string& qs)
	{
		static std::regex qs_pattern("^&?([^=&]+)=([^&]+)");
		const char* str = qs.c_str();
		size_t size = qs.size();
		uintptr_t pos = (uintptr_t)qs.find("?");
		nlohmann::json results{};

		if (pos == (uintptr_t)std::string::npos)
		{
			pos = 0;
		}
		else
		{
			pos++;
		}

		while (pos < size)
		{
			std::cmatch matches;
			std::regex_search(str + pos, matches, qs_pattern);

			if (!matches.empty())
			{
				if (results.contains(matches[1].str()))
				{
					results[matches[1].str()].push_back(matches[2].str());
				}
				else
				{
					results[matches[1].str()] = {matches[2].str()};
				}
				pos += matches.length();
			}
			else
			{
				break;
			}
		}

		return results;
	}
}