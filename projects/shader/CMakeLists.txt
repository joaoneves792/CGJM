cmake_minimum_required(VERSION 3.8.0)
project(CGJMshader CXX)

include_directories(
	includes
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${CGJMvector_INCLUDE_DIRS}
	${CGJMmatrix_INCLUDE_DIRS})

set(SOURCE_FILES
  src/Shader.cpp
  src/glerror.cpp)

set(SHADER_TEST
  tests/shader_test.cpp)


add_executable(shaderTest ${SOURCE_FILES} ${SHADER_TEST})
target_link_libraries(shaderTest ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} CGJMvector CGJMmatrix)
add_dependencies(shaderTest InstallFreeGLUT)

add_custom_command(TARGET shaderTest POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:shaderTest>/res)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/includes
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
