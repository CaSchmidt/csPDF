set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms)

set(csPDF_FORMS
  forms/wmainwindow.ui
  forms/wquicksearch.ui
  )

set(csPDF_HEADERS
  include/wmainwindow.h
  include/wquicksearch.h
  )

set(csPDF_SOURCES
  src/main.cpp
  src/wmainwindow.cpp
  src/wquicksearch.cpp
  )

add_executable(csPDF
  ${csPDF_HEADERS} ${csPDF_SOURCES}
  )

target_link_libraries(csPDF csPDFium csPDFUI Qt5::Widgets)

format_output_name(csPDF "csPDF")

set_target_properties(csPDF PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${csPDF_BINARY_DIR}
  )

if(WIN32)
  set_target_properties(csPDF PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_include_directories(csPDF PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(csPDF PRIVATE ${csPDF_SOURCE_DIR}/csPDFium/include)
target_include_directories(csPDF PRIVATE ${csPDF_SOURCE_DIR}/csPDFSearch/include)
target_include_directories(csPDF PRIVATE ${csPDF_SOURCE_DIR}/csPDFUI/include)

install(TARGETS csPDF
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )
