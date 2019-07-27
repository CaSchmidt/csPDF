### Project ##################################################################

list(APPEND crzlib_SOURCES
  src/adler32.c
  src/adler32_simd.c
  src/compress.c
  src/contrib/optimizations/inffast_chunk.c
  src/contrib/optimizations/inflate.c
  src/crc32.c
  src/crc32_simd.c
  src/crc_folding.c
  src/deflate.c
  src/fill_window_sse.c
  src/gzclose.c
  src/gzlib.c
  src/gzread.c
  src/gzwrite.c
  src/infback.c
  src/inffast.c
  src/inftrees.c
  src/trees.c
  src/uncompr.c
  src/x86.c
  src/zutil.c
  )

list(APPEND crzlib_DEFINES
  ADLER32_SIMD_SSSE3
  CRC32_SIMD_SSE42_PCLMUL
  INFLATE_CHUNK_SIMD_SSE2
  ZLIB_IMPLEMENTATION
  )

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  list(APPEND crzlib_DEFINES
    INFLATE_CHUNK_READ_64LE
    )
endif()

### Target ###################################################################

add_library(crzlib STATIC
  ${crzlib_SOURCES}
  )

format_output_name(crzlib "Cr-zlib")

target_compile_definitions(crzlib
  PRIVATE ${crzlib_DEFINES}
  )

target_include_directories(crzlib
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

install(TARGETS crzlib
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(FILES include/chromeconf.h include/zconf.h include/zlib.h
  DESTINATION include
  )
