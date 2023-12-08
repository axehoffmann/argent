add_executable(engine "Main.cpp" "audio/AudioSource.h" "misc/FuncUtils.h" "lib/vector.h" "lib/ptr.h" "engine/engine.h" "engine/game_thread.h" "engine/worker_thread.h")

add_subdirectory(debug)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(rendering)
add_subdirectory(resources)
add_subdirectory(lib)

target_link_libraries(engine core lib ecs renderer resources log_print)