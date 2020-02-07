### Project ##################################################################

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

### Target ###################################################################

add_executable(csPDF
  ${csPDF_HEADERS} ${csPDF_SOURCES}
  )

format_output_name(csPDF "csPDF")

if(WIN32)
  set_target_properties(csPDF PROPERTIES
    WIN32_EXECUTABLE ON
    )
endif()

target_include_directories(csPDF
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csPDF csPDFUI)

set_target_properties(csPDF PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
  )

### Install ##################################################################

install(TARGETS csPDF
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )
