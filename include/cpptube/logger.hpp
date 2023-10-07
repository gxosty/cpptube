#pragma once

#include <string>
#include <iostream>

enum CPPTUBE_LOGGER_LEVEL {
	CPPTUBE_LOGGER_LEVEL_NONE     = 0,
	CPPTUBE_LOGGER_LEVEL_INFO     = 1 << 1,
	CPPTUBE_LOGGER_LEVEL_DEBUG    = 1 << 2,
	CPPTUBE_LOGGER_LEVEL_WARNING  = 1 << 3,
	CPPTUBE_LOGGER_LEVEL_ERROR    = 1 << 4
};

namespace cpptube::logger
{
	class LogPrinter
	{
	private:
		bool __is_enabled;
	public:
		LogPrinter();
		void set_enabled(bool enabled);
		void print_info(CPPTUBE_LOGGER_LEVEL level, const std::string& filename);

		void print(const std::string& str);
		void print(const char* str);
		void print(const int& i);
		void print(const float& f);
		void print(const double& f);
		
		void print(std::ostream&(*pManip)(std::ostream&));
	};

	class Logger
	{
	private:
		std::string __filename;
		LogPrinter __disabled_printer;
		LogPrinter __printer;

		LogPrinter& __get_logprinter(CPPTUBE_LOGGER_LEVEL level);

	public:
		static unsigned level;
		static void set_level(unsigned llevel);

		Logger(const std::string& filename);
		LogPrinter& info();
		LogPrinter& debug();
		LogPrinter& warning();
		LogPrinter& error();
	};

	LogPrinter& operator<<(LogPrinter& printer, const std::string& str);
	LogPrinter& operator<<(LogPrinter& printer, const char* str);
	LogPrinter& operator<<(LogPrinter& printer, const int& i);
	LogPrinter& operator<<(LogPrinter& printer, const float& f);
	LogPrinter& operator<<(LogPrinter& printer, const double& d);

	LogPrinter& operator<<(LogPrinter& printer, std::ostream&(*pManip)(std::ostream&));
}