cmake_minimum_required(VERSION 2.8.4)
project(CGJM C CXX)

if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -Wextra -pedantic")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/x64)
add_subdirectory(projects/vector) 
add_subdirectory(projects/matrix)

#target_link_libraries(cg45-proj ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
#target_link_libraries(cg45-proj GL GLU glut)

