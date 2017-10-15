# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindFREEGLUT
# --------
#
# Find the OpenGL Extension Wrangler Library (FREEGLUT)
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` target ``FREEGLUT::FREEGLUT``,
# if FREEGLUT has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   FREEGLUT_INCLUDE_DIRS - include directories for FREEGLUT
#   FREEGLUT_LIBRARIES - libraries to link against FREEGLUT
#   FREEGLUT_FOUND - true if FREEGLUT has been found and can be used

find_path(FREEGLUT_INCLUDE_DIR GL/freeglut.h)

if(NOT FREEGLUT_LIBRARY)
  find_library(FREEGLUT_LIBRARY_RELEASE NAMES FREEGLUT freeglut SUFFIXES lib64 libx32)

  include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
  select_library_configurations(FREEGLUT)
endif ()

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(FREEGLUT
                                  REQUIRED_VARS FREEGLUT_INCLUDE_DIR FREEGLUT_LIBRARY)

if(FREEGLUT_FOUND)
  set(FREEGLUT_INCLUDE_DIRS ${FREEGLUT_INCLUDE_DIR})

  if(NOT FREEGLUT_LIBRARIES)
    set(FREEGLUT_LIBRARIES ${FREEGLUT_LIBRARY})
  endif()

  if (NOT TARGET FREEGLUT::FREEGLUT)
    add_library(FREEGLUT::FREEGLUT UNKNOWN IMPORTED)
    set_target_properties(FREEGLUT::FREEGLUT PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${FREEGLUT_INCLUDE_DIRS}")

    if(FREEGLUT_LIBRARY_RELEASE)
      set_property(TARGET FREEGLUT::FREEGLUT APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(FREEGLUT::FREEGLUT PROPERTIES IMPORTED_LOCATION_RELEASE "${FREEGLUT_LIBRARY_RELEASE}")
    endif()

    if(FREEGLUT_LIBRARY_DEBUG)
      set_property(TARGET FREEGLUT::FREEGLUT APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(FREEGLUT::FREEGLUT PROPERTIES IMPORTED_LOCATION_DEBUG "${FREEGLUT_LIBRARY_DEBUG}")
    endif()

    if(NOT FREEGLUT_LIBRARY_RELEASE AND NOT FREEGLUT_LIBRARY_DEBUG)
      set_property(TARGET FREEGLUT::FREEGLUT APPEND PROPERTY IMPORTED_LOCATION "${FREEGLUT_LIBRARY}")
    endif()
  endif()
endif()

mark_as_advanced(FREEGLUT_INCLUDE_DIR)
