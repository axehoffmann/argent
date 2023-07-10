add_executable(asset_test  "Resources_ITest.cpp")
target_link_libraries(asset_test PRIVATE resources test_framework)

add_executable(blueprint_test  "Blueprint_ITest.cpp")
target_link_libraries(blueprint_test PRIVATE ecs rendering resources core test_framework)