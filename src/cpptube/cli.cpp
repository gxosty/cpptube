#include <cpptube/cpptube.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	cpptube::YouTube yt("https://youtube.com/watch?v=9bZkp7q19f0");
	cpptube::stream::StreamQuery streams = yt.streams();

	std::cout << streams->first() << std::endl;

	return 0;
}