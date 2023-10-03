#include <cpptube/cpptube.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/innertube.hpp>
#include <cpptube/exceptions.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	cpptube::YouTube yt("https://youtube.com/watch?v=9bZkp7q19f0");
	// cpptube::stream::StreamQuery streams = yt.streams();
	// std::cout << streams->first() << std::endl;

	std::cout << cpptube::helpers::parse_qs("foo=bar&hello=world&you=i123") << std::endl;

	return 0;
}