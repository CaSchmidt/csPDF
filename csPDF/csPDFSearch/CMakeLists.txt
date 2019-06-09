set(csPDFSearch_HEADERS
  include/csPDFSearch/csPdfSearch.h
  include/csPDFSearch/csPdfSearchResult.h
  include/csPDFSearch/csPdfSearchResultsModel.h
  include/csPDFSearch/csPdfSearchUtil.h
  include/csPDFSearch/cspdfsearch_config.h
  include/internal/config_Search.h
  )

set(csPDFSearch_SOURCES
  src/csPdfSearch.cpp
  src/csPdfSearchResultsModel.cpp
  src/csPdfSearchUtil.cpp
  )

add_library(csPDFSearch SHARED
  ${csPDFSearch_HEADERS} ${csPDFSearch_SOURCES}
  )

target_link_libraries(csPDFSearch csPDFium Qt5::Widgets)

format_output_name(csPDFSearch "csPDFSearch")

target_compile_definitions(csPDFSearch PRIVATE CS_BUILD_PDFSEARCH_LIB)
target_include_directories(csPDFSearch PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(csPDFSearch PRIVATE ${csPDF_SOURCE_DIR}/csPDFium/include)

install(TARGETS csPDFSearch
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(DIRECTORY include/
  DESTINATION include
  PATTERN "internal" EXCLUDE
  )
