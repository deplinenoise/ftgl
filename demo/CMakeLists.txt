# -*- CMake -*-
#
# Copyright (C) 2012 Guofu Xiang <gfxiang@gmail.com>.
# All rights reserved.

add_executable(simple simple.cpp)
target_link_libraries(simple
  ftgl ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(c-demo c-demo.c)
target_link_libraries(c-demo
  ftgl ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(FTGLDemo
  FTGLDemo.cpp tb.h tb.c trackball.h trackball.c)
target_link_libraries(FTGLDemo
  ftgl ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(FTGLMFontDemo
  FTGLMFontDemo.cpp tb.h tb.c trackball.h trackball.c)
target_link_libraries(FTGLMFontDemo
  ftgl ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
