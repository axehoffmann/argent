add_executable(renderer "main.cpp" "vertex.h")

add_subdirectory(ogl)

target_link_libraries(renderer PRIVATE log_print lib ${glfw} ${glew} ${ogl32lib})