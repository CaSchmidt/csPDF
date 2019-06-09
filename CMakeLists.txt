cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(csPDF-dev)

file(MAKE_DIRECTORY
  ${csPDF-dev_BINARY_DIR}/bin
  ${csPDF-dev_BINARY_DIR}/lib
  )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${csPDF-dev_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${csPDF-dev_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY     ${csPDF-dev_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${csPDF-dev_BINARY_DIR}/bin)

add_subdirectory(csPDF)
