# -*- CMake -*-
#
# Copyright (C) 2012 Guofu Xiang <gfxiang@gmail.com>.
# All rights reserved.

file(GLOB FTGL_HEADERS FTGL/*.h)
file(GLOB FTGL_FTFONT_SOURCES FTFont/*.h FTFont/*.cpp)
file(GLOB FTGL_FTGLYPH_SOURCES FTGlyph/*.h FTGlyph/*.cpp)
file(GLOB FTGL_FTLAYOUT_SOURCES FTLayout/*.h FTLayout/*.cpp)
file(GLOB _FTGL_SOURCES *.h *.cpp)

set(FTGL_SOURCES
  ${_FTGL_SOURCES}
  ${FTGL_HEADERS}
  ${FTGL_FTFONT_SOURCES}
  ${FTGL_FTGLYPH_SOURCES}
  ${FTGL_FTLAYOUT_SOURCES})

add_library(ftgl ${FTGL_SOURCES})
target_link_libraries(ftgl ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARIES})

install(TARGETS ftgl
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)

install(FILES ${FTGL_HEADERS} DESTINATION include/FTGL)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION include/FTGL)
