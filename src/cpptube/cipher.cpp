#include <cpptube/cipher.hpp>
#include <cpptube/parser.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/exceptions.hpp>
#include <cpptube/logger.hpp>

#include <regex>
#include <vector>
#include <algorithm>
#include <fstream>

namespace cpptube::cipher
{
	cpptube::logger::Logger logger(__FILE__);

	Cipher::Cipher(const std::string& js)
	{
		std::ofstream js_file("js.js");
		js_file << js;
		js_file.close();

		this->__transform_plan = get_transform_plan(js);
		std::regex var_regex("^\\w+\\W");
		std::smatch matches;
		std::regex_search((const std::string&)this->__transform_plan[0].get<std::string>(), matches, var_regex);

		if (matches.empty())
		{
			throw cpptube::exceptions::RegexMatchError("Cipher constructor", "^\\w+\\W");
		}

		std::string var = matches[0].str().substr(0, matches.length(0) - 1);
		logger.debug() << "Var: " << var << std::endl;
		this->__transform_map = get_transform_map(js, var);
		this->__throttling_plan = get_throttling_plan(js);		
		this->__throttling_array = get_throttling_function_array(js);

		this->__calculated_n.clear();		
	}

	std::string& Cipher::calculate_n(const std::string& initial_n)
	{
		if (!this->__calculated_n.empty())
		{
			return this->__calculated_n;
		}

		nlohmann::json initial_n_json{};

		for (auto& c : initial_n)
		{
			initial_n_json += std::string(1, c);
		}

		for (int i = 0; i < this->__throttling_array.size(); i++)
		{
			if (this->__throttling_array[i].get<std::string>() == "b")
			{
				this->__throttling_array[i] = (uintptr_t)&initial_n_json;
			}
		}

		for (auto& step : this->__throttling_plan.items())
		{
			CPPTUBE_THROTTLING_FUNCTIONS curr_func = (CPPTUBE_THROTTLING_FUNCTIONS)this->__throttling_array[std::stoi(step.value()[0].get<std::string>())].get<int>();
			nlohmann::json* first_arg = reinterpret_cast<nlohmann::json*>(this->__throttling_array[std::stoi(step.value()[1].get<std::string>())].get<uintptr_t>());

			switch (curr_func)
			{
			case CPPTUBE_THROTTLING_FUNCTION_REVERSE:
				{
					cpptube::cipher::throttling_reverse(first_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_UNSHIFT:
				{
					int second_arg = this->__throttling_array[std::stoi(step.value()[2].get<std::string>())].get<int>();
					cpptube::cipher::throttling_unshift(first_arg, second_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_CIPHER_FUNCTION:
				{
					std::string second_arg = this->__throttling_array[std::stoi(step.value()[2].get<std::string>())].get<std::string>();
					cpptube::cipher::throttling_cipher_function(first_arg, second_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_NESTED_SPLICE:
				{
					int second_arg = this->__throttling_array[std::stoi(step.value()[2].get<std::string>())].get<int>();
					cpptube::cipher::throttling_nested_splice(first_arg, second_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_PREPEND:
				{
					int second_arg = this->__throttling_array[std::stoi(step.value()[2].get<std::string>())].get<int>();
					cpptube::cipher::throttling_prepend(first_arg, second_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_SWAP:
				{
					int second_arg = this->__throttling_array[std::stoi(step.value()[2].get<std::string>())].get<int>();
					cpptube::cipher::throttling_swap(first_arg, second_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_JS_SPLICE:
				{
					int second_arg = this->__throttling_array[std::stoi(step.value()[2].get<std::string>())].get<int>();
					cpptube::cipher::js_splice(first_arg, second_arg);
				}

				break;

			case CPPTUBE_THROTTLING_FUNCTION_PUSH:
				{
					first_arg->push_back(this->__throttling_array[std::stoi(step.value()[2].get<std::string>())]);
				}

				break;
			default:
				logger.error() << "curr_func == " << (int)curr_func << std::endl;
				logger.error() << "Throttling_array: " << this->__throttling_array << std::endl;
				throw cpptube::exceptions::ExtractError("curr_func == " + std::to_string(curr_func));
			}
		}

		this->__calculated_n.clear();

		for (auto& item : initial_n_json.items())
		{
			this->__calculated_n = __calculated_n + item.value().get<std::string>();
		}

		return this->__calculated_n;
	}

	std::string Cipher::get_signature(const std::string& ciphered_signature)
	{
		nlohmann::json signature = {};

		for (auto& js_func : this->__transform_plan.items())
		{
			std::pair<std::string, int> name_argument = this->parse_function(js_func.value().get<std::string>());
			CPPTUBE_TRANSFORM_FUNCTIONS transform_func = (CPPTUBE_TRANSFORM_FUNCTIONS)this->__transform_map[name_argument.first].get<int>();

			switch (transform_func)
			{
			case CPPTUBE_TRANSFORM_FUNCTION_REVERSE:
				{
					signature = cpptube::cipher::reverse(signature);
				}

				break;
			case CPPTUBE_TRANSFORM_FUNCTION_SPLICE:
				{
					signature = cpptube::cipher::splice(signature, name_argument.second);
				}

				break;
			case CPPTUBE_TRANSFORM_FUNCTION_SWAP:
				{
					signature = cpptube::cipher::swap(signature, name_argument.second);
				}

				break;
			default:
				logger.error() << "transform_func = " << (int)transform_func << std::endl;
				throw cpptube::exceptions::ExtractError("transform_func == " + std::to_string(transform_func));
			}

			logger.debug()
			<< "Applied transform function" << std::endl
			<< "output: " << signature << std::endl
			<< "js_function: " << name_argument.first << std::endl
			<< "argument: " << name_argument.second << std::endl
			<< "function: " << (int)transform_func << std::endl;
		}

		std::string signature_str;

		for (auto& item : signature.items())
		{
			signature_str = signature_str + item.value().get<std::string>();
		}

		return signature_str;
	}

	std::pair<std::string, int> Cipher::parse_function(const std::string& js_func)
	{
		logger.debug() << "Parsing transform function" << std::endl;

		static const int js_func_patterns_size = 2;
		static const std::string js_func_patterns[] = {
			"\\w+\\.(\\w+)\\(\\w,(\\d+)\\)",
			"\\w+\\[(\"\\w+\")\\]\\(\\w,(\\d+)\\)"
		};

		for (int i = 0; i < js_func_patterns_size; i++)
		{
			std::regex pattern(js_func_patterns[i]);
			std::smatch matches;
			std::regex_search(js_func, matches, pattern);

			if (!matches.empty())
			{
				int arg = 0;

				if (matches.size() > 2)
				{
					arg = std::stoi(matches[2].str());
				}

				return {matches[1].str(), arg};
			}
		}

		throw cpptube::exceptions::RegexMatchError(
			"parse_function", "js_func_patterns"
		);
	}

	std::string get_initial_function_name(const std::string& js)
	{
		static const int function_patterns_size = 12;
		static const std::string function_patterns[] = {
			"\\b[cs]\\s*&&\\s*[adf]\\.set\\([^,]+\\s*,\\s*encodeURIComponent\\s*\\(\\s*([a-zA-Z0-9$]+)\\(",
			"\\b[a-zA-Z0-9]+\\s*&&\\s*[a-zA-Z0-9]+\\.set\\([^,]+\\s*,\\s*encodeURIComponent\\s*\\(\\s*([a-zA-Z0-9$]+)\\(",
			"(?:\\b|[^a-zA-Z0-9$])([a-zA-Z0-9$]{2})\\s*=\\s*function\\(\\s*a\\s*\\)\\s*\\{\\s*a\\s*=\\s*a\\.split\\(\\s*\"\"\\s*\\)",
			"([a-zA-Z0-9$]+)\\s*=\\s*function\\(\\s*a\\s*\\)\\s*\\{\\s*a\\s*=\\s*a\\.split\\(\\s*\"\"\\s*\\)",
			"([\"\'])signature\\1\\s*,\\s*([a-zA-Z0-9$]+)\\(",
			"\\.sig\\|\\|([a-zA-Z0-9$]+)\\(",
			"yt\\.akamaized\\.net/\\)\\s*\\|\\|\\s*.*?\\s*[cs]\\s*&&\\s*[adf]\\.set\\([^,]+\\s*,\\s*(?:encodeURIComponent\\s*\\()?\\s*(?:[a-zA-Z0-9$]+)\\(",
			"\\b[cs]\\s*&&\\s*[adf]\\.set\\([^,]+\\s*,\\s*([a-zA-Z0-9$]+)\\(",
			"\\b[a-zA-Z0-9]+\\s*&&\\s*[a-zA-Z0-9]+\\.set\\([^,]+\\s*,\\s*([a-zA-Z0-9$]+)\\(",
			"\\bc\\s*&&\\s*a\\.set\\([^,]+\\s*,\\s*\\([^)]*\\)\\s*\\(\\s*([a-zA-Z0-9$]+)\\(",
			"\\bc\\s*&&\\s*[a-zA-Z0-9]+\\.set\\([^,]+\\s*,\\s*\\([^)]*\\)\\s*\\(\\s*([a-zA-Z0-9$]+)\\(",
			"\\bc\\s*&&\\s*[a-zA-Z0-9]+\\.set\\([^,]+\\s*,\\s*\\([^)]*\\)\\s*\\(\\s*([a-zA-Z0-9$]+)\\(",
		};

		logger.debug() << "Finding initial function name" << std::endl;

		for (int i = 0; i < function_patterns_size; i++)
		{
			logger.debug() << "Trying pattern: " << function_patterns[i] << std::endl;
			std::regex pattern(function_patterns[i]);
			std::smatch matches;
			std::regex_search(js, matches, pattern);

			if (!matches.empty())
			{
				logger.debug() << "Finished regex search, matched: " << function_patterns[i] << std::endl;
				logger.debug() << "Initial function name: " << matches[1].str() << std::endl;
				return matches[1].str();
			}
		}

		throw cpptube::exceptions::RegexMatchError(
			"get_initial_function_name", "multiple"
		);
	}

	nlohmann::json get_transform_plan(const std::string& js)
	{
		std::string name = cpptube::helpers::escape_for_regex(get_initial_function_name(js));
		std::string pattern_str = name + "=function\\(\\w\\)\\{[a-z=\\.\\(\"\\)]*;(.*);(?:.+)\\}";
		logger.debug() << "Getting transform plan" << std::endl;
		return cpptube::helpers::split_string(cpptube::helpers::regex_search(pattern_str, js, 1), ";");
	}

	nlohmann::json get_transform_object(const std::string& js, const std::string& var)
	{
		logger.debug() << "Getting transform object" << std::endl;
		std::string pattern_str = "var " + cpptube::helpers::escape_for_regex(var) + "=\\{([\\s\\S\\n]*?)\\};"; // there is no `re.DOTALL` equivalent in cpp
		logger.debug() << "Trying pattern: " << pattern_str << std::endl;
		std::regex pattern(pattern_str);
		std::smatch matches;
		std::regex_search(js, matches, pattern);

		if (matches.empty())
		{
			throw cpptube::exceptions::RegexMatchError(
				"get_transform_object", pattern_str
			);
		}

		logger.debug() << "Finished regex search, matched: " << pattern_str << std::endl;

		std::string transform_match = matches[1].str();
		std::replace_if(transform_match.begin(), transform_match.end(), [](char c) -> bool {
			return c == '\n';
		}, ' ');

		return cpptube::helpers::split_string(transform_match, ", ");
	}

	nlohmann::json get_transform_map(const std::string& js, const std::string& var)
	{
		logger.debug() << "Getting transform map" << std::endl;
		nlohmann::json transform_object = cpptube::cipher::get_transform_object(js, var);
		logger.debug() << "transform_object: " << transform_object << std::endl;
		nlohmann::json mapper = {};

		for (auto& obj : transform_object.items())
		{
			logger.debug() << obj.value().get<std::string>() << std::endl;
			nlohmann::json name_function = cpptube::helpers::split_string(obj.value().get<std::string>(), ":", 1);
			CPPTUBE_TRANSFORM_FUNCTIONS fn = cpptube::cipher::map_functions(name_function[1].get<std::string>());
			mapper[name_function[0].get<std::string>()] = (int)fn;
		}

		return mapper;
	}

	std::string get_throttling_function_name(const std::string& js)
	{
		static const int function_patterns_size = 1;
		static const std::string function_patterns[] = {
			"a\\.[a-zA-Z]\\s*&&\\s*\\([a-z]\\s*=\\s*a\\.get\\(\"n\"\\)\\)\\s*&&\\s*" //! pay attention there is no comma
			"\\([a-z]\\s*=\\s*([a-zA-Z0-9$]+)(\\[\\d+\\])?\\([a-z]\\)",
		};

		logger.debug() << "Finding throttling function name" << std::endl;

		for (int i = 0; i < function_patterns_size; i++)
		{
			std::regex pattern(function_patterns[i]);
			std::smatch matches;
			std::regex_search(js, matches, pattern);

			if (!matches.empty())
			{
				logger.debug() << "Finished regex search, matched: " << function_patterns[i] << std::endl;

				if (matches.size() == 1)
				{
					logger.debug() << "Function that computes throttling parameter: " << matches[1].str() << std::endl;
					return matches[1].str();
				}

				std::string idx = matches[2].str();

				if (!idx.empty())
				{
					idx = idx.substr(1, idx.size() - 2); // stripping "[]"
					logger.debug() << "Finding index" << std::endl;
					logger.debug() << "Trying pattern: " << "var " + cpptube::helpers::escape_for_regex(matches[1].str()) + "\\s*=\\s*(\\[.+?\\]);" << std::endl;
					std::regex pattern2("var " + cpptube::helpers::escape_for_regex(matches[1].str()) + "\\s*=\\s*(\\[.+?\\]);");
					std::regex_search(js, matches, pattern2);

					if (!matches.empty())
					{
						std::string idx2 = matches[1].str();
						idx2 = idx2.substr(1, idx2.size() - 2);
						nlohmann::json arr = cpptube::helpers::split_string(idx2, ",");
						nlohmann::json arr2 = {};

						for (auto& item : arr.items())
						{
							arr2 += cpptube::helpers::strip_string(item.value().get<std::string>());
						}

						logger.debug() << "Function that computes throttling parameter: " << arr2[std::stoi(idx)].get<std::string>() << std::endl;

						return arr2[std::stoi(idx)].get<std::string>();
					}
				}
			}
		}

		throw cpptube::exceptions::RegexMatchError(
			"get_throttling_function_name", "multiple"
		);
	}

	std::string get_throttling_function_code(const std::string& js)
	{
		logger.debug() << "Getting throttling function code" << std::endl;

		std::string name = cpptube::helpers::escape_for_regex(cpptube::cipher::get_throttling_function_name(js));
		logger.debug() << "Function code regex: " << name + "=function\\(\\w\\)" << std::endl;
		std::regex pattern(name + "=function\\(\\w\\)");
		std::smatch matches;
		std::regex_search(js, matches, pattern);

		nlohmann::json code_lines_list = cpptube::helpers::split_string(
			cpptube::parser::find_object_from_startpoint(
				&js,
				(matches.position(0) + matches.length(0))
			),
			"\n"
		);

		std::string joined_lines;

		for (auto& item : code_lines_list.items())
		{
			joined_lines = joined_lines + item.value().get<std::string>();
		}

		return matches[0].str() + joined_lines;
	}

	nlohmann::json get_throttling_function_array(const std::string& js)
	{
		logger.debug() << "Getting throttling function array" << std::endl;
		std::string raw_code = cpptube::cipher::get_throttling_function_code(js);

		std::regex array_start_pattern(",c=\\[");
		std::smatch matches;
		std::regex_search(raw_code, matches, array_start_pattern);

		std::string array_raw = cpptube::parser::find_object_from_startpoint(&raw_code, (matches.position(0) + matches.length(0) - 1));
		nlohmann::json str_array = cpptube::parser::throttling_array_split(array_raw);

		nlohmann::json converted_array = {};

		for (auto& item : str_array.items())
		{
			std::string el = item.value().get<std::string>();

			try {
				converted_array += std::stoi(el);
				continue;
			} catch (...) {}

			if (el == "null")
			{
				converted_array += 0;
				continue;
			}

			if ((el[0] == '"') && (el[el.size() - 1] == '"'))
			{
				converted_array += el.substr(1, el.size() - 2);
				continue;
			}

			if (el.find("function") != std::string::npos)
			{
				static const std::vector<std::pair<std::string, CPPTUBE_THROTTLING_FUNCTIONS>> mapper = {
					{"\\{for\\(\\w=\\(\\w%\\w\\.length\\+\\w\\.length\\)%\\w\\.length;\\w--;\\)\\w\\.unshift\\(\\w.pop\\(\\)\\)\\}", CPPTUBE_THROTTLING_FUNCTION_UNSHIFT},
					{"\\{\\w\\.reverse\\(\\)\\}", CPPTUBE_THROTTLING_FUNCTION_REVERSE},
					{"\\{\\w\\.push\\(\\w\\)\\}", CPPTUBE_THROTTLING_FUNCTION_PUSH},
					{";var\\s\\w=\\w\\[0\\];\\w\\[0\\]=\\w\\[\\w\\];\\w\\[\\w\\]=\\w\\}", CPPTUBE_THROTTLING_FUNCTION_SWAP},
					{"case\\s\\d+", CPPTUBE_THROTTLING_FUNCTION_CIPHER_FUNCTION},
					{"\\w\\.splice\\(0,1,\\w\\.splice\\(\\w,1,\\w\\[0\\]\\)\\[0\\]\\)", CPPTUBE_THROTTLING_FUNCTION_NESTED_SPLICE},
					{";\\w\\.splice\\(\\w,1\\)\\}", CPPTUBE_THROTTLING_FUNCTION_JS_SPLICE},
					{"\\w\\.splice\\(-\\w\\)\\.reverse\\(\\)\\.forEach\\(function\\(\\w\\)\\{\\w\\.unshift\\(\\w\\)\\}\\)", CPPTUBE_THROTTLING_FUNCTION_PREPEND},
					{"for\\(var \\w=\\w\\.length;\\w;\\)\\w\\.push\\(\\w\\.splice\\(--\\w,1\\)\\[0\\]\\)\\}", CPPTUBE_THROTTLING_FUNCTION_REVERSE}
				};

				bool found = false;

				// logger.debug() << "`el` starts with \"function\"";
				for (auto& pattern_fn : mapper)
				{
					// logger.debug() << "Trying pattern: " << pattern_fn.first << std::endl;
					std::regex pattern(pattern_fn.first);
					std::smatch match;
					if (std::regex_search(el, match, pattern))
					{
						converted_array += pattern_fn.second;
						found = true;
					}
				}
			}

			converted_array += el;
		}

		return converted_array;
	}

	nlohmann::json get_throttling_plan(const std::string& js)
	{
		logger.debug() << "Getting throttling plan" << std::endl;

		std::string raw_code = cpptube::cipher::get_throttling_function_code(js);
		static const std::regex plan_pattern("try\\{");
		std::smatch match;
		std::regex_search(raw_code, match, plan_pattern);

		std::string transform_plan_raw = cpptube::parser::find_object_from_startpoint(&raw_code, (match.position(0) + match.length(0) - 1));

		static const std::string step_start_pattern = "c\\[(\\d+)\\]\\(c\\[(\\d+)\\](,c(\\[(\\d+)\\]))?\\)";

		std::vector<std::smatch> matches = cpptube::helpers::regex_search_all(step_start_pattern, transform_plan_raw);
		nlohmann::json transform_steps = {};
		for (int i = 0; i < matches.size(); i++)
		{
			std::smatch& match = matches[i];
			if (match[4].str() != "")
			{
				transform_steps += {match[0].str(), match[1].str(), match[4].str()};
			}
			else
			{
				transform_steps += {match[0].str(), match[1].str()};
			}
		}

		return transform_steps;
	}

	nlohmann::json reverse(const nlohmann::json& list)
	{
		nlohmann::json output = {};

		for (int i = list.size(); i >= 0; i--)
		{
			output += list.at(i);
		}

		return output;
	}

	nlohmann::json splice(const nlohmann::json& list, int b)
	{
		nlohmann::json output = {};

		for (int i = b; i < list.size(); i++)
		{
			output += list[i];
		}

		return output;
	}

	nlohmann::json swap(const nlohmann::json& list, int b)
	{
		int r = b % (int)list.size();
		nlohmann::json output = {};

		output += list[r];

		for (int i = 1; i < r; i++)
		{
			output += list[i];
		}

		output += list[0];

		for (int i = r + 1; i < list.size(); i++)
		{
			output += list[i];
		}

		return output;
	}

	void throttling_reverse(nlohmann::json* list)
	{
		nlohmann::json reverse_list = {};

		for (int i = list->size() - 1; i >= 0; i--)
		{
			reverse_list += (*list)[i];
		}

		*list = reverse_list;
	}

	int throttling_mod_func(const nlohmann::json& list, int e)
	{
		return (e % list.size() + list.size()) % list.size();
	}

	void throttling_unshift(nlohmann::json* list, int e)
	{
		e = cpptube::cipher::throttling_mod_func(*list, e);
		nlohmann::json new_arr = {};

		for (int i = list->size() - e; i < list->size(); i++)
		{
			new_arr += list->at(i);
		}

		for (int i = 0; i < list->size() - e; i++)
		{
			new_arr += list->at(i);
		}

		*list = new_arr;
	}

	void throttling_cipher_function(nlohmann::json* list, std::string e)
	{
		static const std::string h = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
		int f = 96;

		int i = 0;
		for (auto& item : list->items())
		{
			int bracket_val = (h.find(item.value().get<std::string>()) - h.find(std::string(1, e[i])) + i - 32 + f) % h.size();
			e.push_back(h[bracket_val]);
			list->at(i) = std::string(1, h[bracket_val]);
			f -= 1;
			i++;
		}
	}

	void throttling_nested_splice(nlohmann::json* list, int e)
	{
		e = cpptube::cipher::throttling_mod_func(*list, e);
		nlohmann::json inner_splice = cpptube::cipher::js_splice(
			list,
			e,
			1,
			{list->at(0)}
		);

		cpptube::cipher::js_splice(
			list,
			0,
			1,
			{inner_splice.at(0)}
		);
	}

	void throttling_prepend(nlohmann::json* list, int e)
	{
		size_t start_len = list->size();
		e = cpptube::cipher::throttling_mod_func(*list, e);

		nlohmann::json new_arr = {};

		for (int i = list->size() - e; i < list->size(); i++)
		{
			new_arr += list->at(i);
		}

		for (int i = 0; i < list->size() - e; i++)
		{
			new_arr += list->at(i);
		}

		*list = new_arr;

		// Unfortunately, I am not going to put assertion here
	}

	void throttling_swap(nlohmann::json* list, int e)
	{
		e = cpptube::cipher::throttling_mod_func(*list, e);
		std::string f = list->at(0);
		list->at(0) = list->at(e);
		list->at(e) = f;
	}

	nlohmann::json js_splice(nlohmann::json* list, int start, int delete_count, nlohmann::json items)
	{
		if (start > list->size())
		{
			start = (int)list->size();
		}

		if (start < 0)
		{
			start = (int)list->size() - start;
		}

		if (delete_count >= list->size() - start)
		{
			delete_count = list->size() - start;
		}

		nlohmann::json deleted_elements = {};

		for (int i = start; i < start + delete_count; i++)
		{
			deleted_elements += list->at(i);
		}

		nlohmann::json new_arr = {};

		for (int i = 0; i < start; i++)
		{
			new_arr += list->at(i);
		}

		for (auto& item : items.items())
		{
			new_arr += item.value();
		}

		for (int i = start + delete_count; i < list->size(); i++)
		{
			new_arr += list->at(i);
		}

		*list = new_arr;

		return deleted_elements;
	}

	CPPTUBE_TRANSFORM_FUNCTIONS map_functions(const std::string& js_func)
	{
		logger.debug() << "Mapping transform function" << std::endl;
		static const std::vector<std::pair<std::string, CPPTUBE_TRANSFORM_FUNCTIONS>> mapper = {
			{"\\{\\w\\.reverse\\(\\)\\}", CPPTUBE_TRANSFORM_FUNCTION_REVERSE},
			{"\\{\\w\\.splice\\(0,\\w\\)\\}", CPPTUBE_TRANSFORM_FUNCTION_SPLICE},
			{"\\{var\\s\\w=\\w\\[0\\];\\w\\[0\\]=\\w\\[\\w\\%\\w\\.length\\];\\w\\[\\w\\]=\\w\\}", CPPTUBE_TRANSFORM_FUNCTION_SWAP},
			{"\\{var\\s\\w=\\w\\[0\\];\\w\\[0\\]=\\w\\[\\w\\%\\w\\.length\\];\\w\\[\\w\\%\\w\\.length\\]=\\w\\}", CPPTUBE_TRANSFORM_FUNCTION_SWAP},
		};

		for (auto& pattern_fn : mapper)
		{
			std::regex pattern(pattern_fn.first);
			std::smatch matches;
			std::regex_search(js_func, matches, pattern);

			if (!matches.empty())
			{
				return pattern_fn.second;
			}
		}

		throw cpptube::exceptions::RegexMatchError(
			"map_functions", "multiple"
		);
	}
}