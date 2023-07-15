target_sources(tests PRIVATE "Transform_UTest.cpp" "Grid_UTest.cpp" "CVARs_UTest.cpp")
add_subdirectory(structures)
target_link_libraries(tests core)