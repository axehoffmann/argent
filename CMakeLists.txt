cmake_minimum_required (VERSION 3.12)

project ("argent")

include(CTest)

file(STRINGS includes.txt INCLUDE_DIRS)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
find_library(criterion criterion HINTS ${INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tests)