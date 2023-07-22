add_subdirectory(debug)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(rendering)
add_subdirectory(resources)

add_executable(engine "Main.cpp" "audio/AudioSource.h")
target_link_libraries(engine core ecs rendering ogl resources)