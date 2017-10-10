cmake_minimum_required(VERSION 2.8.4)
project(CGJMmatrix C CXX)

include_directories(includes 
	${CGJMvector_INCLUDE_DIRS})

set(HEADER_FILES
  ${CGJMvector_INCLUDE_DIRS}/vec.h
  includes/mat.h)

set(SOURCE_FILES
  src/Mat2.cpp 
  src/Mat3.cpp 
  src/Mat4.cpp)

set(MAT2_TEST
  tests/mat2Test.cpp)

set(MAT3_TEST
  tests/mat3Test.cpp)

set(MAT4_TEST
  tests/mat4Test.cpp)


add_executable(mat2Test ${SOURCE_FILES} ${HEADER_FILES} ${MAT2_TEST})
add_executable(mat3Test ${SOURCE_FILES} ${HEADER_FILES} ${MAT3_TEST})
add_executable(mat4Test ${SOURCE_FILES} ${HEADER_FILES} ${MAT4_TEST})
target_link_libraries(mat2Test CGJMvector)
target_link_libraries(mat3Test CGJMvector)
target_link_libraries(mat4Test CGJMvector)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/includes
	${CGJMvector_INCLUDE_DIRS}
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
