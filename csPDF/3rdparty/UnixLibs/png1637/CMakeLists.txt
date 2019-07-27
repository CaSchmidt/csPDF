### Project ##################################################################

list(APPEND png_SOURCES
  src/png.c
  src/pngerror.c
  src/pngget.c
  src/pngmem.c
  src/pngpread.c
  src/pngread.c
  src/pngrio.c
  src/pngrtran.c
  src/pngrutil.c
  src/pngset.c
  src/pngtrans.c
  src/pngwio.c
  src/pngwrite.c
  src/pngwtran.c
  src/pngwutil.c
  src/intel/filter_sse2_intrinsics.c
  src/intel/intel_init.c
  )

### Target ###################################################################

add_library(png STATIC
  ${png_SOURCES}
  )

format_output_name(png "png")

target_compile_definitions(png
  PRIVATE PNG_INTEL_SSE_OPT=1
  )

target_include_directories(png
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${zlib_INCLUDES}
  )

target_link_libraries(png crzlib)
