### Project ##################################################################

list(APPEND zlib_SOURCES
  src/adler32.c
  src/compress.c
  src/crc32.c
  src/deflate.c
  src/gzclose.c
  src/gzlib.c
  src/gzread.c
  src/gzwrite.c
  src/infback.c
  src/inffast.c
  src/inflate.c
  src/inftrees.c
  src/trees.c
  src/uncompr.c
  src/zutil.c
  )

if(MSVC)
  enable_language(ASM_MASM)
  set(CMAKE_ASM_MASM_FLAGS "${CMAKE_ASM_MASM_FLAGS} /nologo")
  message(${CMAKE_ASM_MASM_SOURCE_FILE_EXTENSIONS})
  set(CMAKE_ASM_MASM_SOURCE_FILE_EXTENSIONS masm CACHE STRING "" FORCE)
  message(${CMAKE_ASM_MASM_SOURCE_FILE_EXTENSIONS})
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    list(APPEND zlib_SOURCES
      src-masm64/gvmat64.masm
      src-masm64/inffas8664.c
      src-masm64/inffasx64.masm
      )
    list(APPEND zlib_DEFINES
      ASMV
      ASMINF
      )
    set_source_files_properties(src-masm64/gvmat64.masm src-masm64/inffasx64.masm
      PROPERTIES LANGUAGE ASM_MASM
      )
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    list(APPEND zlib_SOURCES
      src-masm86/inffas32.masm
      src-masm86/match686.masm
      )
    list(APPEND zlib_DEFINES
      ASMV
      ASMINF
      )
    set_source_files_properties(src-masm86/inffas32.masm src-masm86/match686.masm
      PROPERTIES LANGUAGE ASM_MASM
      )
  endif()
endif()

### Target ###################################################################

add_library(zlib STATIC
  ${zlib_SOURCES}
  )

if(DEFINED zlib_DEFINES)
  target_compile_definitions(zlib
    PRIVATE ${zlib_DEFINES}
    )
endif()

target_include_directories(zlib
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

target_link_libraries(zlib)
