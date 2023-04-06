add_library(ogl "GLRenderEngine.h" "GLScreen.h" "GLScreen.cpp")
target_link_libraries(ogl PRIVATE rendering opengl glfw glew resources)
