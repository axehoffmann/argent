target_sources(tests PRIVATE "Resources_ITest.cpp"  "Mesh_ITest.cpp" "Texture_ITest.cpp" "Materials_ITest.cpp" "Blueprint_ITest.cpp")
target_link_libraries(tests resources)