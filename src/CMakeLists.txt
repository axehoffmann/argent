add_executable(engine "Main.cpp" "audio/AudioSource.h" "misc/FuncUtils.h" "lib/vector.h" "lib/ptr.h" "engine/engine.h" "engine/game_thread.h" "engine/worker_thread.h")

add_executable(agpk "resources/v2/packer/agpk.cpp")

add_custom_target(assets 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(agpk assets)
add_dependencies(engine assets)

add_subdirectory(debug)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(rendering)
add_subdirectory(resources)
add_subdirectory(lib)

target_link_libraries(engine core lib ecs renderer resources log_print)
target_link_libraries(agpk PRIVATE rv2 log_print)