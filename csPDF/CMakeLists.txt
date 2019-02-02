cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(csPDF)

### Modules ##################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(FormatOutputName)

### Dependencies #############################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms)
find_package(Qt5Widgets 5.6 REQUIRED)

find_library(PDFium_LIB NAMES PDFium PDFium-x86 PDFium-x64)
if(NOT PDFium_LIB)
  message(FATAL_ERROR "PDFium library not found!")
else()
  message("PDFium library found (${PDFium_LIB}).")
endif()

add_subdirectory(../csQt/csQt
  ${CMAKE_CURRENT_BINARY_DIR}/csQt
  )

set_target_properties(csQt PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${csPDF_BINARY_DIR}
  )

### Targets ##################################################################

add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII)

add_subdirectory(csPDFium)
add_subdirectory(csPDFSearch)
add_subdirectory(csPDFUI)
add_subdirectory(examples/csPDF)
