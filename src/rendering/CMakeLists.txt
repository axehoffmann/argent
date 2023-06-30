add_library(rendering Renderer.cpp SceneBuilder.cpp "StaticRenderable.h" "StaticRenderable.cpp")
add_subdirectory(opengl)

# add_subdirectory(lighting)
# target_link_libraries(rendering PRIVATE lighting)
