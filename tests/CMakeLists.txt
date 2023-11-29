add_executable(tests "TestFramework.h" "TestFramework.cpp" "../src/debug/log/ConsoleWriter.h" "test_framework.h")
target_link_libraries(tests log_test)

add_subdirectory(lib)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(resources)
