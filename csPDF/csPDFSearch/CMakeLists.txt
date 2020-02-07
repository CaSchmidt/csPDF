### Project ##################################################################

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

### Target ###################################################################

add_library(csPDFSearch SHARED
  ${csPDFSearch_HEADERS} ${csPDFSearch_SOURCES}
  )

format_output_name(csPDFSearch "csPDFSearch")

target_compile_definitions(csPDFSearch
  PRIVATE CS_BUILD_PDFSEARCH_LIB
  )

target_include_directories(csPDFSearch
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(csPDFSearch
  PUBLIC csPDFium
  )

set_target_properties(csPDFSearch PROPERTIES
  AUTOMOC ON
  )

### Install ##################################################################

install(TARGETS csPDFSearch
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(DIRECTORY include/
  DESTINATION include
  PATTERN "internal" EXCLUDE
  )
