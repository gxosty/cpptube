#include <cpptube/cpptube.hpp>
#include <cpptube/innertube.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	cpptube::YouTube yt("https://youtube.com/watch?v=9bZkp7q19f0");
	// cpptube::stream::StreamQuery streams = yt.streams();
	// std::cout << streams->first() << std::endl;

	innertube::InnerTube it;
	std::ofstream file("test.json");
	file << std::setw(4) << it.player(yt.video_id) << std::endl;
	file.close();

	return 0;
}