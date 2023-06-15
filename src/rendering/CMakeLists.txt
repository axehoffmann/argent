add_library(rendering Renderer.cpp SceneBuilder.cpp "lighting/PointLight.h" "lighting/DirectionalLight.h")
add_subdirectory(opengl lighting)
target_link_libraries(rendering PRIVATE lighting)
