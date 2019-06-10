cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(UnixLibs)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

set(ENABLE_SHARED  OFF CACHE BOOL "" FORCE)
set(WITH_CRT_DLL   ON  CACHE BOOL "" FORCE)
set(WITH_JAVA      OFF CACHE BOOL "" FORCE)
set(WITH_JPEG8     ON  CACHE BOOL "" FORCE)
set(WITH_SIMD      ON  CACHE BOOL "" FORCE)
set(WITH_TURBOJPEG OFF CACHE BOOL "" FORCE)

set(ft2_INCLUDES    ${CMAKE_CURRENT_SOURCE_DIR}/ft291/include/freetype2)
list(APPEND jt_INCLUDES
  ${csPDF_BINARY_DIR}/UnixLibs/jt202
  ${CMAKE_CURRENT_SOURCE_DIR}/jt202
  )
set(pdfium_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/pdfium2785/public PARENT_SCOPE)
set(png_INCLUDES    ${CMAKE_CURRENT_SOURCE_DIR}/png1637/include)
set(zlib_INCLUDES   ${CMAKE_CURRENT_SOURCE_DIR}/zlib1211/include)

add_subdirectory(zlib1211)
add_subdirectory(png1637)
add_subdirectory(ft291)
add_subdirectory(jt202)
add_subdirectory(pdfium2785)
