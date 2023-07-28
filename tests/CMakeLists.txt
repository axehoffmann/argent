add_executable(tests "TestFramework.h" "TestFramework.cpp" "../src/debug/log/ConsoleWriter.h")
target_compile_definitions(tests PRIVATE AG_LOGGING_CONSOLE=0)

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(resources)