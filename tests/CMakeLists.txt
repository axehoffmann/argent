add_executable(tests "TestFramework.h" "TestFramework.cpp" "../src/debug/log/ConsoleWriter.h")

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(resources)