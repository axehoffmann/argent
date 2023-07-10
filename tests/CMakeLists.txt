add_library(test_framework "TestFramework.h" "TestFramework.cpp")

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(resources)

add_executable(new_test_test "NewTestFramework.h" "TestTest.cpp" "NewTestFramework.cpp")