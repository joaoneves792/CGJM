cmake_minimum_required(VERSION 3.8.0)
project(CGJM C CXX)

if( MSVC )
	list(APPEND CMAKE_PREFIX_PATH 
		${CGJM_SOURCE_DIR}/dependencies/glew-2.1.0/
		${CGJM_SOURCE_DIR}/dependencies/freeglut/ )
endif (MSVC)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)


if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif (UNIX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/x64)
add_subdirectory(projects/vector) 
add_subdirectory(projects/matrix)
add_subdirectory(projects/shader)
