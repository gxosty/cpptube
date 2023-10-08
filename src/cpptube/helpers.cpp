#include <cpptube/helpers.hpp>
#include <cpptube/exceptions.hpp>
#include <curl/curl.h>

#include <regex>
#include <iostream>
#include <algorithm>

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

	std::vector<std::smatch> regex_search_all(const std::string& pattern, const std::string& str)
	{
		std::regex regex_pattern(pattern);
		std::smatch matches;
		std::vector<std::smatch> res{};

		std::string::const_iterator sstart(str.cbegin());

		while (std::regex_search(sstart, str.cend(), matches, regex_pattern))
		{
			res.push_back(matches);
			sstart = matches.suffix().first;
		}

		return res;
	}

	std::string url_encode(const std::string& str)
	{
		char* encoded_str = curl_escape(str.c_str(), str.size());
		// TODO: throw exception if `encoded_url` is :nullptr:
		std::string estr = std::string(encoded_str);
		free(encoded_str);
		return estr;
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

	nlohmann::json split_string(std::string str, std::string del, int count)
	{
		// Strips right spaces, because I don't think we need em
		str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(), str.end());

		if (str.length() == 0) {
			return {};
		}

		nlohmann::json output{};
		int start = 0, end = 0;
		while ((true) && (count != 0)) {
			end = str.find(del, start);
			if (end == std::string::npos) {
				break;
			}

			if (end != start) {
				output += (str.substr(start, end - start));
			}

			start = end + del.size();
			count--;
		}
		output += (str.substr(start, str.length() - start));
		return output;
	}

	std::string strip_string(const std::string& str, const std::string& strip)
	{
		uintptr_t start = (uintptr_t)&*str.begin();
		uintptr_t end = (uintptr_t)&*str.end();

		auto _start = std::find_if(str.begin(), str.end(), [&strip](char c) -> bool {
			return std::find(strip.begin(), strip.end(), c) == strip.end();
		});

		auto _end = std::find_if(str.rbegin(), str.rend(), [&strip](char c) -> bool {
			return std::find(strip.begin(), strip.end(), c) == strip.end();
		});

		if (_start != str.end())
		{
			start = (uintptr_t)&*_start;
		}

		if (_end != str.rend())
		{
			end = (uintptr_t)&*_end + 1;
		}

		if (start > end)
		{
			return "";
		}

		return std::string((char*)start, end - start);
	}


	std::string escape_for_regex(const std::string& str)
	{
		static const char metacharacters[] = R"(\.^$-+()[]{}|?*)";
		std::string out;
		out.reserve(str.size());
		for (auto ch : str) {
			if (std::strchr(metacharacters, ch))
				out.push_back('\\');
			out.push_back(ch);
		}
		return out;
	}
}