add_library(test_framework "TestFramework.h" "TestFramework.cpp")

add_executable(archetype_test  "archetype_test.cpp")
target_link_libraries(archetype_test PRIVATE ecs test_framework)

add_executable(world_test  "world_test.cpp")
target_link_libraries(world_test PRIVATE ecs test_framework)

add_executable(transform_test  "transform_test.cpp")
target_link_libraries(transform_test PRIVATE core test_framework)

add_executable(component_test  "component_test.cpp")
target_link_libraries(component_test PRIVATE ecs test_framework)