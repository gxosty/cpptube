#pragma once

#include <cpptube/logger.hpp>
#include <nlohmann/json.hpp>

#include <string>
#include <pair>

namespace cpptube::cipher
{
	cpptube::logger::Logger logger(__FILE__);

	class Cipher
	{
	private:
		nlohmann::json __transform_plan;

		nlohmann::json __throttling_plan;
		nlohmann::json __throttling_array;

		std::string __calculated_n;

	public:
		Cipher(const std::string& js);

		std::string& calculate_n(const std::string& initial_n);
		std::string get_signature(const std::string& ciphered_signature);
		std::pair<std::string, int> parse_function(const std::string& js_func);
	}

	std::string get_initial_function_name(const std::string& str);
	nlohmann::json get_transform_plan(const std::string& js);
	nlohmann::json get_transform_object(const std::string& js, const std::string& var);
	nlohmann::json get_transform_map(const std::string& js, const std::string& var);
	std::string get_throttling_function_name(const std::string& js);
	std::string get_throttling_function_code(const std::string& js);
	nlohmann::json get_throttling_function_array(const std::string& js);
	nlohmann::json get_throttling_plan(const std::string& js);
	nlohmann::json reverse(const nlohmann::json& list);
	nlohmann::json splice(const nlohmann::json& list, int b);
	nlohmann::json swap(const nlohmann::json& list, int b);

	void throttling_reverse(nlohmann::json* list);
	// void throttling_push(nlohmann::json* list, any e); | we don't need this...
	int throttling_mod_func(const nlohmann::json& list, int e);
	void throttling_unshift(nlohmann::json* list, int e);
	void throttling_cipher_function(nlohmann::json* list, std::string e);
	void throttling_nested_splice(nlohmann::json* list, int e);
	void throttling_prepend(nlohmann::json* list, int e);
	void throttling_swap(nlohmann::json* list, int e);
	nlohmann::json js_splice(nlohmann::json* list, int start, int delete_count = 0, nlohmann::json items = {});
	void* map_functions(const std::string& js_func);
}