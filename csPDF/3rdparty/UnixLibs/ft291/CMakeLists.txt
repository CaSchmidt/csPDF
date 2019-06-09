### Project ##################################################################

list(APPEND freetype_SOURCES
  src/ftsystem.c
  src/ftinit.c
  src/ftdebug.c
  src/ftbase.c
  src/ftbbox.c
  src/ftglyph.c
  src/ftbdf.c
  src/ftbitmap.c
  src/ftcid.c
  src/ftfstype.c
  src/ftgasp.c
  src/ftgxval.c
  src/ftmm.c
  src/ftotval.c
  src/ftpatent.c
  src/ftpfr.c
  src/ftstroke.c
  src/ftsynth.c
  src/fttype1.c
  src/ftwinfnt.c
  src/bdf.c
  src/cff.c
  src/type1cid.c
  src/pcf.c
  src/pfr.c
  src/sfnt.c
  src/truetype.c
  src/type1.c
  src/type42.c
  src/winfnt.c
  src/raster.c
  src/smooth.c
  src/autofit.c
  src/psaux.c
  src/pshinter.c
  src/psnames.c
  src/ftlzw.c
  src/ftgzip.c
  )

### Target ###################################################################

add_library(freetype STATIC
  ${freetype_SOURCES}
  )

target_compile_definitions(freetype
  PRIVATE FT2_BUILD_LIBRARY
  )

target_include_directories(freetype
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/freetype2
  PRIVATE ${zlib_INCLUDES}
  )

target_link_libraries(freetype zlib)
