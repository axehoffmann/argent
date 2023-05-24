add_library(test_framework "TestFramework.h" "TestFramework.cpp")

add_executable(world_test  "world_test.cpp")
target_link_libraries(world_test PRIVATE ecs test_framework)

add_executable(system_test  "system_test.cpp")
target_link_libraries(system_test PRIVATE ecs test_framework)

add_executable(archetype_test  "archetype_test.cpp")
target_link_libraries(archetype_test PRIVATE ecs test_framework)

add_executable(transform_test  "transform_test.cpp")
target_link_libraries(transform_test PRIVATE core test_framework)

add_executable(component_test  "component_test.cpp")
target_link_libraries(component_test PRIVATE ecs test_framework)

add_executable(grid_test  "grid_test.cpp")
target_link_libraries(grid_test PRIVATE ecs test_framework)

add_executable(asset_test  "asset_test.cpp")
target_link_libraries(asset_test PRIVATE resources test_framework)

add_executable(cvar_test  "cvar_test.cpp")
target_link_libraries(cvar_test PRIVATE core test_framework)