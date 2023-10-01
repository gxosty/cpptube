cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
# `cpptube` - a port of pytube

set(PROJECT_NAME cpptube)
set(CPPTUBE_EXECUTABLE_NAME ${PROJECT_NAME}-bin)
set(CPPTUBE_SHARED_LIB_NAME ${PROJECT_NAME}-lib-shared)
set(CPPTUBE_STATIC_LIB_NAME ${PROJECT_NAME}-lib-static)

project(${PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(${CPPTUBE_SHARED_LIB_NAME} SHARED
	src/cpptube/cpptube.cpp
	src/cpptube/streams.cpp
	src/cpptube/monostate.cpp
	src/cpptube/request.cpp
	src/cpptube/query.cpp
	src/cpptube/extract.cpp
	src/cpptube/helpers.cpp
	src/cpptube/innertube.cpp
)

target_link_libraries(${CPPTUBE_SHARED_LIB_NAME}
	stdc++fs

	z
	ssl
	crypto
	curl
)

set_target_properties(${CPPTUBE_SHARED_LIB_NAME}
	PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)

add_executable(${CPPTUBE_EXECUTABLE_NAME}
	src/cpptube/cli.cpp
)

target_link_libraries(${CPPTUBE_EXECUTABLE_NAME}
	${CPPTUBE_SHARED_LIB_NAME}
)

set_target_properties(${CPPTUBE_EXECUTABLE_NAME}
	PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)