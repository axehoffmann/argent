find_package(meshoptimizer CONFIG REQUIRED)

add_executable(agpk 
	"agpk.cpp"
 "mesh.cpp" "mesh.h")

target_link_libraries(agpk PRIVATE meshoptimizer::meshoptimizer)