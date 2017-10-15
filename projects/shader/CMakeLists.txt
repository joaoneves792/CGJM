cmake_minimum_required(VERSION 3.8.0)
project(CGJMshader CXX)

include_directories(
	includes
	${GLEW_INCLUDE_DIR}
	${FREEGLUT_INCLUDE_DIR})

message(${GLEW_INCLUDE_DIRS})
message(${FREEGLUT_INCLUDE_DIR})


set(SOURCE_FILES
  src/Shader.cpp)

set(SHADER_TEST
  tests/shader_test.cpp)

add_executable(shaderTest ${SOURCE_FILES} ${SHADER_TEST})
target_link_libraries(shaderTest ${GLEW_LIBRARIES} ${FREEGLUT_LIBRARIES})
#target_link_libraries(mat4Test CGJMvector)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/includes
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
