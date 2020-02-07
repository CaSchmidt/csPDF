cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(csPDF)

### Modules ##################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(FormatOutputName)

### Dependencies #############################################################

find_package(Qt5Widgets 5.6 REQUIRED)

add_subdirectory(3rdparty/UnixLibs
  ${CMAKE_CURRENT_BINARY_DIR}/UnixLibs
  )

format_output_name(PDFium "PDFium")

add_subdirectory(../csQt/csQt
  ${CMAKE_CURRENT_BINARY_DIR}/csQt
  )

### Targets ##################################################################

add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII)

add_subdirectory(csPDFium)
add_subdirectory(csPDFSearch)
add_subdirectory(csPDFUI)
add_subdirectory(examples/csPDF)
