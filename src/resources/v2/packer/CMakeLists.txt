find_package(meshoptimizer CONFIG REQUIRED)

add_executable(agpk 
	"agpk.cpp"
)

 add_library(rv2 "mesh.cpp" "mesh.h")

target_link_libraries(rv2 PRIVATE meshoptimizer::meshoptimizer)
target_link_libraries(agpk PRIVATE rv2)