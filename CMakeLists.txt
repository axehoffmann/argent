﻿cmake_minimum_required (VERSION 3.12)

project ("argent")

include(CTest)

file(STRINGS includes.txt INCLUDE_DIRS)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})

find_library(ogl32lib OpenGL32 HINTS ${INCLUDE_DIRS})
find_library(glfw glfw3 HINTS ${INCLUDE_DIRS})
find_library(glew glew32s HINTS ${INCLUDE_DIRS})

if(NOT glfw OR NOT ogl32lib OR NOT glew)
  message(FATAL_ERROR "gl library not found")
endif()

add_subdirectory(src)
add_subdirectory(tests)