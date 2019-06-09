cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(UnixLibs)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

set(ft2_INCLUDES  ${CMAKE_CURRENT_SOURCE_DIR}/ft291/include)
set(png_INCLUDES  ${CMAKE_CURRENT_SOURCE_DIR}/png1637/include)
set(zlib_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/zlib1211/include)

add_subdirectory(zlib1211)
add_subdirectory(png1637)
add_subdirectory(ft291)
