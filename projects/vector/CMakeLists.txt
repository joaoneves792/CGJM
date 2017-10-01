cmake_minimum_required(VERSION 2.8.4)
project(CGJMvector C CXX)

include_directories(includes)

#find_package(Threads REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)

set(HEADER_FILES
  includes/vec.h)

set(SOURCE_FILES
  src/Vec2.cpp
  src/Vec3.cpp
  tests/vecTest.cpp)



if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -Wextra -pedantic")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/x64)

add_executable(vecTest ${SOURCE_FILES} ${HEADER_FILES})
#target_link_libraries(cg45-proj ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
#target_link_libraries(cg45-proj GL GLU glut)

