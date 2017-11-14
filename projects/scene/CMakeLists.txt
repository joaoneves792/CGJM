cmake_minimum_required(VERSION 3.2.2)
project(CGJMscene CXX)

include_directories(
	includes/
	${GLEW_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${CGJMvector_INCLUDE_DIRS}
	${CGJMmatrix_INCLUDE_DIRS}
	${CGJMshader_INCLUDE_DIRS}
    ${CGJMquaternions_INCLUDE_DIRS})

set(SOURCE_FILES
        src/OBJMesh.cpp
		src/SphereCamera.cpp
		src/FreeCamera.cpp
		src/Camera.cpp
		src/SceneGraph.cpp)
set(TEST_FILES
		tests/scene_viewer.cpp
		tests/scene.cpp
		tests/animation.cpp
		)

add_executable(scene_viewer ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(scene_viewer ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} CGJMvector CGJMmatrix CGJMshader CGJMquaternions)
add_dependencies(scene_viewer InstallFreeGLUT)

add_custom_command(TARGET scene_viewer POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		   ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:shaderTest>/res)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/includes
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
