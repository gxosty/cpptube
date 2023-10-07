#include <cpptube/logger.hpp>
#include <cpptube/definitions.hpp>

#include <iostream>
#include <ctime>
#include <iomanip>

namespace cpptube::logger
{
	LogPrinter::LogPrinter()
	{
		this->__is_enabled = true;
	}

	void LogPrinter::set_enabled(bool enabled)
	{
		this->__is_enabled = enabled;
	}

	void LogPrinter::print_info(CPPTUBE_LOGGER_LEVEL level, const std::string& filename)
	{
		time_t t = std::time(0);
		auto tm = std::localtime(&t);

		std::cout << std::put_time(tm, "[%H:%M:%S]");

		switch (level)
		{
		case CPPTUBE_LOGGER_LEVEL_INFO:
			std::cout << "[INFO]";
			break;
		case CPPTUBE_LOGGER_LEVEL_DEBUG:
			std::cout << "[DEBUG]";
			break;
		case CPPTUBE_LOGGER_LEVEL_WARNING:
			std::cout << "[WARNING]";
			break;
		case CPPTUBE_LOGGER_LEVEL_ERROR:
			std::cout << "[ERROR]";
			break;
		default:
			std::cout << "[UNKNOWN]";
			break;
		}

		std::cout << "[" << filename << "] ";
	}

	void LogPrinter::print(const std::string& str)
	{
		if (this->__is_enabled)
			std::cout << str;
	}

	void LogPrinter::print(const char* str)
	{
		if (this->__is_enabled)
			std::cout << str;
	}

	void LogPrinter::print(const int& i)
	{
		if (this->__is_enabled)
			std::cout << i;
	}

	void LogPrinter::print(const float& f)
	{
		if (this->__is_enabled)
			std::cout << f;
	}

	void LogPrinter::print(const double& d)
	{
		if (this->__is_enabled)
			std::cout << d;
	}

	void LogPrinter::print(std::ostream&(*pManip)(std::ostream&))
	{
		if (this->__is_enabled)
			std::cout << *pManip;
	}

	unsigned Logger::level = CPPTUBE_LOGGER_LEVEL_NONE;

	void Logger::set_level(unsigned llevel)
	{
		Logger::level = llevel;
	}

	Logger::Logger(const std::string& filename)
	{
		fs::path filepath(filename);
		this->__filename = filepath.filename().string();
		this->__disabled_printer.set_enabled(false);
	}

	LogPrinter& Logger::__get_logprinter(CPPTUBE_LOGGER_LEVEL level)
	{
		if (Logger::level & level)
		{
			this->__printer.print_info(level, this->__filename);
			return this->__printer;
		}

		return this->__disabled_printer;
	}

	LogPrinter& Logger::info()
	{
		return this->__get_logprinter(CPPTUBE_LOGGER_LEVEL_INFO);
	}

	LogPrinter& Logger::debug()
	{
		return this->__get_logprinter(CPPTUBE_LOGGER_LEVEL_DEBUG);
	}

	LogPrinter& Logger::warning()
	{
		return this->__get_logprinter(CPPTUBE_LOGGER_LEVEL_WARNING);
	}

	LogPrinter& Logger::error()
	{
		return this->__get_logprinter(CPPTUBE_LOGGER_LEVEL_ERROR);
	}

	LogPrinter& operator<<(LogPrinter& printer, const std::string& str)
	{
		printer.print(str);
		return printer;
	}

	LogPrinter& operator<<(LogPrinter& printer, const char* str)
	{
		printer.print(str);
		return printer;
	}

	LogPrinter& operator<<(LogPrinter& printer, const int& i)
	{
		printer.print(i);
		return printer;
	}

	LogPrinter& operator<<(LogPrinter& printer, const float& f)
	{
		printer.print(f);
		return printer;
	}

	LogPrinter& operator<<(LogPrinter& printer, const double& d)
	{
		printer.print(d);
		return printer;
	}

	LogPrinter& operator<<(LogPrinter& printer, std::ostream&(*pManip)(std::ostream&))
	{
		printer.print(pManip);
		return printer;
	}
}