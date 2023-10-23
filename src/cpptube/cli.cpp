#include <cpptube/definitions.hpp>
#include <cpptube/cpptube.hpp>
#include <cpptube/query.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/parser.hpp>
#include <cpptube/innertube.hpp>
#include <cpptube/exceptions.hpp>
#include <cpptube/logger.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv)
{
	cpptube::set_logger_level(CPPTUBE_LOGGER_LEVEL_INFO | CPPTUBE_LOGGER_LEVEL_DEBUG | CPPTUBE_LOGGER_LEVEL_WARNING | CPPTUBE_LOGGER_LEVEL_ERROR);
	
	cpptube::YouTube yt("https://youtube.com/watch?v=9bZkp7q19f0");
	cpptube::query::StreamQuery streams = yt.streams();
	std::cout << streams.size() << std::endl;

	auto filtered_streams = streams.filter({
		{"fps", 24}
	});

	std::cout << filtered_streams.size() << std::endl;

	// std::cout << streams->first() << std::endl;

	return 0;
}