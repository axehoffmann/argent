add_executable(renderer "main.cpp" "vertex.h" "transform.h" "transform.cpp" "glm.h")

add_subdirectory(ogl)

target_link_libraries(renderer PRIVATE log_print lib resources ${glfw} ${glew} ${ogl32lib})