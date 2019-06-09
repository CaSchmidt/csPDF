cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(UnixLibs)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

set(zlib_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/zlib1211/include)

add_subdirectory(zlib1211)
