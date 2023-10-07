#include <cpptube/parser.hpp>
#include <cpptube/exceptions.hpp>

#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace cpptube::parser
{
	std::string find_object_from_startpoint(const std::string* html, unsigned start_point)
	{
		if ((*html)[start_point] != '{')
		{
			if ((*html)[start_point] != '[')
			{
				throw cpptube::exceptions::HTMLParseError(("Invalid start point. Start of HTML:\n" + html->substr(start_point, 20)));
			}
		}

		char last_char = '{';
		char curr_char = 0;
		char in_string = 0;
		std::vector<char> stack = {(*html)[start_point]};
		unsigned i = start_point + 1;

		static std::unordered_map<char, char> context_closers = {
			{'{', '}'},
			{'[', ']'},
			{'"', '"'},
			{'\'', '\''},
			{'/', '/'},
		};

		static const std::vector<char> string_or_regex_chars = {'"', '\'', '/'};
		static const std::vector<char> regex_exception_chars = {'(', ',', '=', ':', '[', '!', '&', '|', '?', '{', '}', ';'};

		while (i < html->length())
		{
			if (stack.empty())
				break;

			if ((curr_char != ' ') && (curr_char != '\n'))
				last_char = curr_char;

			curr_char = (*html)[i];
			char& curr_context = stack.back();

			if (curr_char == context_closers[curr_context])
			{
				if ((in_string == 0) || (in_string == curr_char))
				{
					stack.pop_back();
					i++;
					in_string = 0;
					continue;
				}
			}

			if (std::find(string_or_regex_chars.begin(), string_or_regex_chars.end(), curr_context) != string_or_regex_chars.end())
			{
				if (curr_char == '\\')
				{
					i += 2;
					continue;
				}
			}
			else
			{
				if (context_closers.find(curr_char) != context_closers.end())
				{
					if (!((curr_char == '/') && (std::find(regex_exception_chars.begin(), regex_exception_chars.end(), last_char) == regex_exception_chars.end())))
					{
						stack.push_back(curr_char);
					}
				}
			}

			i++;
		}

		return html->substr(start_point, i - start_point);
	}

	nlohmann::json parse_for_object_from_startpoint(const std::string* html, unsigned start_point)
	{
		std::string full_obj = find_object_from_startpoint(html, start_point);

		try {
			return nlohmann::json::parse(full_obj);
		} catch (...) {}

		throw cpptube::exceptions::HTMLParseError("Couldn't parse object");
	}

	nlohmann::json parse_for_object(const std::string* html, std::string* pattern)
	{
		std::regex _pattern(*pattern);
		std::smatch matches;
		std::regex_search(*html, matches, _pattern);

		if (matches.empty())
			throw cpptube::exceptions::HTMLParseError("No matches for regex " + *pattern);

		std::cout << "pos: " << (unsigned)(matches.position() + matches.length()) << std::endl;
		return parse_for_object_from_startpoint(html, (unsigned)(matches.position() + matches.length()));
	}
}