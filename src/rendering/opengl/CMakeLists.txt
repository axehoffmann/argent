add_library(ogl "GLRenderEngine.h" "GLScreen.h" "GLScreen.cpp" "GLRenderEngine.cpp" "GL.h")
add_subdirectory(structures)
target_link_libraries(ogl PRIVATE rendering resources ${glfw} ${glew} ${ogl32lib})
