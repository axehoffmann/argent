﻿cmake_minimum_required (VERSION 3.13)

project ("argent")

file(STRINGS includes.txt INCLUDE_DIRS)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})

find_library(ogl32lib OpenGL32 HINTS ${INCLUDE_DIRS})
find_library(glew glew32s HINTS ${INCLUDE_DIRS})

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

find_package(meshoptimizer CONFIG REQUIRED)

find_package(Bitsery CONFIG REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Stb REQUIRED)


if(NOT ogl32lib)
 message(FATAL_ERROR "OpenGL32.lib not found")
endif()

if(NOT glew)
 message(FATAL_ERROR "GLEW dll not found")
endif()

add_subdirectory(src)
add_subdirectory(tests) 