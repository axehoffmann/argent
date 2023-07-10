add_executable(world_test  "World_ITest.cpp")
target_link_libraries(world_test PRIVATE ecs test_framework)

add_executable(system_test  "System_UTest.cpp")
target_link_libraries(system_test PRIVATE ecs test_framework)

add_executable(archetype_test  "Archetype_UTest.cpp")
target_link_libraries(archetype_test PRIVATE ecs test_framework)

add_executable(component_test  "Component_UTest.cpp")
target_link_libraries(component_test PRIVATE ecs test_framework)