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
	
	// cpptube::YouTube yt("https://youtube.com/watch?v=9bZkp7q19f0");
	// cpptube::query::StreamQuery streams = yt.streams();

	// cpptube::streams::Stream* stream = streams.get_by_itag(18);

	// stream->download();

	cpptube::request::filesize("https://www.google.com");

	return 0;
}