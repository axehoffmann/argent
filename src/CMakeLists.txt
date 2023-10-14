add_executable(engine "Main.cpp" "audio/AudioSource.h" "misc/FuncUtils.h" "lib/vector.h" "lib/ptr.h")

add_subdirectory(debug)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(rendering)
add_subdirectory(resources)
add_subdirectory(lib)

target_link_libraries(engine core lib ecs rendering ogl resources log_print)