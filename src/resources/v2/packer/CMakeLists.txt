find_package(meshoptimizer CONFIG REQUIRED)

 add_library(rv2 "mesh.cpp" "mesh.h")

target_link_libraries(rv2 PRIVATE meshoptimizer::meshoptimizer tinyobjloader::tinyobjloader Bitsery::bitsery)
target_link_libraries(agpk PRIVATE rv2)