add_executable(tests "../src/debug/log/ConsoleWriter.h" "test_framework.h" "test_framework.cpp")
target_link_libraries(tests log_test)

add_subdirectory(lib)
add_subdirectory(ecs)