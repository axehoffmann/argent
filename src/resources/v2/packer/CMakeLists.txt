find_package(meshoptimizer CONFIG REQUIRED)

add_executable(agpk 
	"agpk.cpp"
 "meshpk.cpp" "meshpk.h")

target_link_libraries(agpk PRIVATE meshoptimizer::meshoptimizer)