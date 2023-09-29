#include <cpptube/cpptube.hpp>
#include <iostream>

int main(int argc, char** argv) {
	cpptube::YouTube yt("https://youtube.com/watch?v=9bZkp7q19f0");

	std::cout << "Object created" << std::endl;

	return 0;
}