cmake_minimum_required(VERSION 2.8.4)
project(CGJMmatrix C CXX)

include_directories(includes 
	${CGJMvector_INCLUDE_DIRS})

set(HEADER_FILES
  ${CGJMvector_INCLUDE_DIRS}/vec.h
  includes/mat.h)

set(SOURCE_FILES
  src/Mat2.cpp)

set(TEST_FILES
  tests/matTest.cpp)


add_executable(matTest ${SOURCE_FILES} ${HEADER_FILES} ${TEST_FILES})
target_link_libraries(matTest CGJMvector)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/includes
	${CGJMvector_INCLUDE_DIRS}
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
