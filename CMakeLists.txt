# -*- CMake -*-
#
# Copyright (C) 2012 Guofu Xiang <gfxiang@gmail.com>.
# All rights reserved.

cmake_minimum_required(VERSION 2.8)

project(ftgl)

set(FTGL_VERSION_MAJOR 2)
set(FTGL_VERSION_MINOR 2)
set(FTGL_VERSION_PATCH 0)
set(FTGL_VERSION
  ${FTGL_VERSION_MAJOR}.${FTGL_VERSION_MINOR}.${FTGL_VERSION_PATCH})

# Freetype
find_package(Freetype REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

find_package(GLUT)

include(CheckIncludeFiles)
check_include_files(gl/gl.h HAVE_GL_GL_H)
check_include_files(OpenGL/gl.h HAVE_OPENGL_GL_H)
check_include_files(gl/glut.h HAVE_GL_GLUT_H)
check_include_files(GLUT/glut.h HAVE_GLUT_GLUT_H)

# Find the default font for FTGL.
find_file(FTGL_FONT_FILE
  NAMES
    DejaVuSerif.ttf VeraSe.ttf DejaVuSans.ttf Vera.ttf
    times.ttf Times.ttf arial.ttf Arial.ttf
  PATHS
    /usr/share/fonts
    /usr/share/fonts/truetype
    /usr/share/fonts/truetype/ttf-dejavu
    /usr/share/fonts/truetype/ttf-bitstream-vera
    /usr/share/fonts/TTF
    /usr/share/fonts/TTF/dejavu
    /usr/share/fonts/dejavu
    /usr/share/fonts/ttf-dejavu
    /usr/share/fonts/ttf-bitstream-vera
    /usr/X11R6/lib/X11/fonts
    /usr/X11R6/lib/X11/fonts/TTF
    C:/Windows/Fonts)
if(FTGL_FONT_FILE)
  message(STATUS "Found default font: ${FTGL_FONT_FILE}")
endif()

option(BUILD_SHARED_LIBS "Build FTGL as a shared library" OFF)
option(FTGL_BUILD_DEMOS "Build demos" OFF)

configure_file(config.h.in config.h @ONLY)

include_directories(
  ${FREETYPE_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  src
  src/FTFont
  src/FTGL
  src/FTGlyph
  src/FTLayout)

add_definitions(-Wall)

add_subdirectory(src)

if(FTGL_BUILD_DEMOS)
  add_subdirectory(demo)
endif()
