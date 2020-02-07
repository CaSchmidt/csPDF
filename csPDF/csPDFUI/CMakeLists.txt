### Project ##################################################################

set(csPDFUI_FORMS
  forms/csPdfUiSearchWidget.ui
  forms/csPdfUiTocWidget.ui
  )

set(csPDFUI_HEADERS
  include/csPDFUI/csPdfUiDocumentView.h
  include/csPDFUI/csPdfUiSearchWidget.h
  include/csPDFUI/csPdfUiTocWidget.h
  include/csPDFUI/cspdfui_config.h
  )

set(csPDFUI_SOURCES
  src/csPdfUiDocumentView.cpp
  src/csPdfUiSearchWidget.cpp
  src/csPdfUiTocWidget.cpp
  )

### Target ###################################################################

add_library(csPDFUI SHARED
  ${csPDFUI_HEADERS} ${csPDFUI_SOURCES}
  )

format_output_name(csPDFUI "csPDFUI")

target_compile_definitions(csPDFUI
  PRIVATE CS_BUILD_PDFUI_LIB
  )

target_include_directories(csPDFUI
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csPDFUI
  PUBLIC csPDFSearch
  PUBLIC csQt
  )

set_target_properties(csPDFUI PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
  )

### Install ##################################################################

install(TARGETS csPDFUI
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(DIRECTORY include/
  DESTINATION include
  PATTERN "internal" EXCLUDE
  )
