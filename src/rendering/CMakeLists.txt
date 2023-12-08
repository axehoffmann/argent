add_library(renderer  "vertex.h" "transform.h" "transform.cpp" "glm.h" "scene_builder.h" "render_object.h" "render_object.cpp" "scene_graph.h" "scene_builder.cpp")

add_subdirectory(ogl)

target_link_libraries(renderer PRIVATE log_print lib resources ${glfw} ${glew} ${ogl32lib})