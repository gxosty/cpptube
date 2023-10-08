#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace cpptube::cipher
{
	enum CPPTUBE_TRANSFORM_FUNCTIONS
	{
		CPPTUBE_TRANSFORM_FUNCTION_REVERSE = 0, // nlohmann::json reverse(const nlohmann::json& list);
		CPPTUBE_TRANSFORM_FUNCTION_SPLICE,      // nlohmann::json splice(const nlohmann::json& list, int b);
		CPPTUBE_TRANSFORM_FUNCTION_SWAP,        // nlohmann::json swap(const nlohmann::json& list, int b);
	};

	enum CPPTUBE_THROTTLING_FUNCTIONS
	{
		CPPTUBE_THROTTLING_FUNCTION_REVERSE = 0,
		CPPTUBE_THROTTLING_FUNCTION_UNSHIFT,
		CPPTUBE_THROTTLING_FUNCTION_CIPHER_FUNCTION,
		CPPTUBE_THROTTLING_FUNCTION_NESTED_SPLICE,
		CPPTUBE_THROTTLING_FUNCTION_PREPEND,
		CPPTUBE_THROTTLING_FUNCTION_SWAP,
		CPPTUBE_THROTTLING_FUNCTION_JS_SPLICE,
		
		CPPTUBE_THROTTLING_FUNCTION_PUSH
	};

	class Cipher
	{
	private:
		nlohmann::json __transform_plan;
		nlohmann::json __transform_map;

		nlohmann::json __throttling_plan;
		nlohmann::json __throttling_array;

		std::string __calculated_n;

	public:
		Cipher(const std::string& js);

		std::string& calculate_n(const std::string& initial_n);
		std::string get_signature(const std::string& ciphered_signature);
		std::pair<std::string, int> parse_function(const std::string& js_func);
	};

	std::string get_initial_function_name(const std::string& js);
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
	nlohmann::json js_splice(nlohmann::json* list, int start, int delete_count = 2'147'483'647, nlohmann::json items = {});
	CPPTUBE_TRANSFORM_FUNCTIONS map_functions(const std::string& js_func);
}