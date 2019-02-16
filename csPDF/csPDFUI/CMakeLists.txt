set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms)

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

add_library(csPDFUI SHARED
  ${csPDFUI_HEADERS} ${csPDFUI_SOURCES}
  )

target_link_libraries(csPDFUI csQt csPDFium csPDFSearch Qt5::Widgets)

format_output_name(csPDFUI "csPDFUI")

set_target_properties(csPDFUI PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${csPDF_BINARY_DIR}
  )

target_compile_definitions(csPDFUI PRIVATE CS_BUILD_PDFUI_LIB)
target_include_directories(csPDFUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(csPDFUI PRIVATE ${csQt_SOURCE_DIR}/include)
target_include_directories(csPDFUI PRIVATE ${csPDF_SOURCE_DIR}/csPDFium/include)
target_include_directories(csPDFUI PRIVATE ${csPDF_SOURCE_DIR}/csPDFSearch/include)

install(TARGETS csPDFUI
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(DIRECTORY include/
  DESTINATION include
  PATTERN "internal" EXCLUDE
  )
