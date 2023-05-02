add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(rendering)
add_subdirectory(resources)

add_executable(engine "Main.cpp")
target_link_libraries(engine core ecs rendering resources)