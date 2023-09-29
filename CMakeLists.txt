cmake_minimum_required(VERSION 3.15)

# `cpptube` - a port of pytube

set(PROJECT_NAME cpptube)

project(${PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME}
	src/cpptube/cli.cpp
)