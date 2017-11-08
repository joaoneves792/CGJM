cmake_minimum_required(VERSION 3.2.2)
project(CGJM3DTangram CXX)

include_directories(
	includes
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${CGJMvector_INCLUDE_DIRS}
	${CGJMmatrix_INCLUDE_DIRS}
	${CGJMshader_INCLUDE_DIRS})

set(SOURCE_FILES
  src/3d_tangram.cpp)

add_executable(3d_tangram ${SOURCE_FILES})
target_link_libraries(3d_tangram ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} CGJMvector CGJMmatrix CGJMshader)
add_dependencies(3d_tangram InstallFreeGLUT)

add_custom_command(TARGET 3d_tangram POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:shaderTest>/res)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/includes
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
