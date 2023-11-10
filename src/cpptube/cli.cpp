#include <cpptube/definitions.hpp>
#include <cpptube/cpptube.hpp>
#include <cpptube/query.hpp>
#include <cpptube/request.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/parser.hpp>
#include <cpptube/innertube.hpp>
#include <cpptube/exceptions.hpp>
#include <cpptube/logger.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

int main(int argc, char** argv)
{
	cpptube::set_logger_level(CPPTUBE_LOGGER_LEVEL_INFO | CPPTUBE_LOGGER_LEVEL_DEBUG | CPPTUBE_LOGGER_LEVEL_WARNING | CPPTUBE_LOGGER_LEVEL_ERROR);
	
	cpptube::YouTube yt("https://www.youtube.com/watch?v=jNQXAC9IVRw");
	cpptube::query::StreamQuery streams = yt.streams();

	cpptube::streams::Stream* stream = streams.get_by_itag(18);

	stream->download("", false, 0, 0, true);

	// std::cout << cpptube::request::filesize("https://www.google.com") << std::endl;

	// std::cout << cpptube::request::__dns_over_https("www.youtube.com") << std::endl;

	return 0;
}