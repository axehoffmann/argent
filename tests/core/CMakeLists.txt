add_executable(transform_test  "Transform_UTest.cpp")
target_link_libraries(transform_test PRIVATE core test_framework)

add_executable(grid_test  "Grid_UTest.cpp")
target_link_libraries(grid_test PRIVATE ecs test_framework)

add_executable(cvar_test  "CVARs_ITest.cpp")
target_link_libraries(cvar_test PRIVATE core test_framework)